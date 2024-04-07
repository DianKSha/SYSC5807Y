/*
    This file is part of the ChipWhisperer Example Targets
    Copyright (C) 2012-2017 NewAE Technology Inc.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "hal.h"
#include <stdint.h>
#include <stdlib.h>

#include "simpleserial.h"


#include "api.h"
#include "ascon.h"
#include "crypto_aead.h"
#include "permutations.h"
#include "printstate.h"
#include "word.h"
#include "aead.c"

#define NONCE_BYTES 16
#define KEY_BYTES 20

static int dataLength = 16; // since the p and ad can be arbitrary long, set the data length first, defualt 16 

static unsigned char key[ KEY_BYTES] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19}; // 20 bytes key for ascon 80pq

static unsigned char* associatedData;// moved to main = (unsigned char*)malloc(16*sizeof(unsigned char));

static unsigned char* plaintext;// moved to main = (unsigned char*)malloc(16*sizeof(unsigned char));

static unsigned char* ciphertext;//  = (unsigned char*)malloc((16+16)*sizeof(unsigned char)); // size(plaintext) + size(nonce)


static uint32_t ciphertextLength = 16; // default 16 bits
static uint32_t plaintextLength = 16; // default 16     bytes
                                           //
static int ciphertextCounter= 0;
static int plaintextCounter = 0;

static uint32_t associatedDataLength = 16; // default 16 bytes

static int associatedDataCounter = 0; // 

static unsigned char nonce[NONCE_BYTES] = {0};

unsigned char * new_ciphertext(uint32_t len);
uint8_t  set_key(uint8_t* k, uint8_t len)
{
	// Load key here
    for (int i = 0; i<KEY_BYTES; i++){
        key[i] = *(k+i);
    }
    
    
    simpleserial_put('r',  20 ,key);
    return 0x00;
}
uint8_t set_nonce(uint8_t* data, uint8_t len)
{
    // set the nonce
    for (int i = 0; i<NONCE_BYTES; i++){
        nonce[i] = *( data+i);
    }
    simpleserial_put('r', 16, nonce);
	return 0x00;
}

uint8_t set_plaintext(uint8_t * data, uint8_t len){
    // set the plaintext 
    
    if(plaintextCounter == (plaintextLength+15)/16)
        return 0; // error, trying to read data when no data should be available
    // read 16 bytes for a time
    int restBytes = plaintextLength - 16*plaintextCounter;
    if(restBytes>=16){
        for(int i = 0; i<16; i++){
            plaintext[i+plaintextCounter*16] = *(data+i);
        }
    }else{
        for(int i = 0; i<restBytes; i++){

            plaintext[i+plaintextCounter*16] = *(data+i);
        }
    }

    simpleserial_put('r', 16, (void*)plaintext+16*plaintextCounter);
    plaintextCounter++;
    return   0x00; 
}

uint8_t set_plaintext_length(uint8_t * data, uint8_t len){
    // set the plaintext length, defualt 16
    // this clears the existing array of plaintext and redeclare one

    plaintextLength = 0;
    for(int i = 0; i<4; i++){
        plaintextLength = 256 * plaintextLength + *(data+i);
    }
    if (plaintext!=NULL) free(plaintext);
    plaintext = (unsigned char*)malloc(plaintextLength*sizeof(unsigned char));
    plaintextCounter = 0;
    // also set the cipher text 
    
    if(ciphertext!=NULL) free(ciphertext);
    ciphertext = new_ciphertext(plaintextLength);
    ciphertextLength = plaintextLength;
    ciphertextCounter =   -1;

    simpleserial_put('r',4,(void*)&plaintextLength);
    return 0x00;
}

uint8_t set_ciphertext(uint8_t * data, uint8_t len){
    // set the ciphertext , should include the tag as the last 16 bytes
    
    if(ciphertextCounter == (ciphertextCounter+15)/16)
        return -1; // error, trying to read data when no data should be available
    // read 16 bytes for a time
    int restBytes = ciphertextLength  - 16*ciphertextCounter;
    if(restBytes>=16){

        for(int i = 0; i<16; i++){
            ciphertext[i+ciphertextCounter*16] = *(data+i);
        }
    }else{
        for(int i = 0; i<restBytes; i++){

            ciphertext[i+ciphertextCounter*16] = *(data+i);
        }
    }
    simpleserial_put('r',16,(void*)ciphertext+ciphertextCounter*16);
    ciphertextCounter++;


    return   0x00; 
}

uint8_t set_ciphertext_length(uint8_t * data, uint8_t len){
    // set the plaintext length, defualt 16
    // this clears the existing array of plaintext and redeclare one

    ciphertextLength = 0;
    for(int i = 0; i<4; i++){
        ciphertextLength = 16*ciphertextLength + *(data+i);
    }
    free(ciphertext);
    ciphertext = (unsigned char*)malloc(plaintextLength*sizeof(unsigned char));

    ciphertextCounter= 0;
    // also set the plaintext
    
    if(plaintext!=NULL) free(plaintext);
    plaintextLength = ciphertextLength - NONCE_BYTES;
    plaintext = (unsigned char*)malloc(plaintextLength * sizeof(unsigned char));
    plaintextCounter = -1;
    simpleserial_put('r', 4, (void*)&ciphertextLength);

    return 0x00;
}


uint8_t set_associated_data(uint8_t * data, uint8_t len){
    // same as plaintext 
    
    if(associatedDataCounter== (associatedDataLength+15)/16)
        return -1; // error, trying to read data when no data should be available
    // read bytes, 16 a time
    int restBytes = associatedDataLength-16*associatedDataCounter;
    if (restBytes>=16){
        for(int i = 0; i<16; i++){
            associatedData[i+associatedDataCounter*16] = *(data+i);
        }
    }else{
        for(int i = 0; i<restBytes; i++){
            associatedData[i+associatedDataCounter*16] = *(data+i);
        }
    }
    simpleserial_put('r',16, (uint8_t*)associatedData+16*associatedDataCounter);
    associatedDataCounter++;
    return   0x00; 
}

uint8_t set_associated_data_length(uint8_t * data, uint8_t len){
    // same as plaintext
    associatedDataLength =  0 ;
    for(int i = 0; i<4; i++){
        
        associatedDataLength = 256*associatedDataLength + *(data+i);
    }
    if(associatedData!=NULL) free(associatedData);
    associatedData = (unsigned char*)malloc(associatedDataLength*sizeof(unsigned char));
    associatedDataCounter=0;

    simpleserial_put('r', 4, (void*)&associatedDataLength);
    return 0x00;
}

unsigned char* new_ciphertext( uint32_t len){
    return (unsigned char*)malloc(len*sizeof(unsigned char));
}
void free_cipher_text(unsigned char * ciphertext){
    free(ciphertext);
}

void encrypt(){
    unsigned long long clen;
    crypto_aead_encrypt(ciphertext, &clen, plaintext, plaintextLength, associatedData, associatedDataLength, (void*)0, nonce, key);
}

int decrypt(){
    
    unsigned long long plen;
    return 
    crypto_aead_decrypt(plaintext, &plen, (void*)0, ciphertext, ciphertextLength+NONCE_BYTES, associatedData, associatedDataLength,nonce, key);

}

uint8_t get_encryption(uint8_t* data, uint8_t len)
{

	/**********************************
	* Start user-specific code here. */
    if(ciphertextCounter==-1){ // -1 indicates encrypt again
        trigger_high();
        encrypt();
        trigger_low();
    }
    ciphertextCounter = (ciphertextCounter+1)%((ciphertextLength+NONCE_BYTES+15)/16); // the last 16 bytes is the tag

	/* End user-specific code here. *
	********************************/
	simpleserial_put('r', 16, (uint8_t*)ciphertext+ciphertextCounter*16);
	return 0x00;
}
uint8_t get_decryption(uint8_t* data, uint8_t len){
    
    // return plaintext 
    if(plaintextCounter==-1){
        trigger_high();
        decrypt();
        trigger_low();
    }
    plaintextCounter = (plaintextCounter+1)%((plaintextLength+15)/16);
    simpleserial_put('r', 16, (uint8_t*)plaintext+plaintextCounter*16);
    return 0x00;

}


uint8_t reset(uint8_t* x, uint8_t len)
{
	// Reset key here if needed
	return 0x00;
}

int main(void)
{
    platform_init();
	init_uart();
	trigger_setup();
    associatedData = (unsigned char*)malloc(16*sizeof(unsigned char));

    plaintext = (unsigned char*)malloc(16*sizeof(unsigned char));

    ciphertext = (unsigned char*)malloc((16+16)*sizeof(unsigned char)); // size(plaintext) + size(nonce)


 	/* Uncomment this to get a HELLO message for debug */
	/*
	putch('h');
	putch('e');
	putch('l');
	putch('l');
	putch('o');
	putch('\n');
	*/

	simpleserial_init();
    // C only holds up to 16 active commands
    // the status of sending back operation status is 
    // ['z', status, '\n']
    //
    // In v1, v,y,w are reversed
    // v: get simpleserial version len=0
    // y: get the number of simpleserial commands on the target
    // w: simple serial commands
    // the handler function must be (uint8_t*, uint8_t) -> uint8_t
    //
    //
    // addcmd(command_char, maximum_len, call_back function)
    // maximum len: up to 64 bytes 
	simpleserial_addcmd('x', 0, reset); // reset
	simpleserial_addcmd('k', 20,  set_key); // set key, 20 bytes
    simpleserial_addcmd('a', 16, set_associated_data); // set associated data
    simpleserial_addcmd('b', 4, set_associated_data_length);// in bits
    simpleserial_addcmd('n', NONCE_BYTES, set_nonce) ;
    simpleserial_addcmd('p', 16, set_plaintext);
    simpleserial_addcmd('q', 4, set_plaintext_length); // in bits
    simpleserial_addcmd('e', 0, get_encryption); // get the encrypted plaintext along with verification tag, 
    simpleserial_addcmd('d', 0, get_decryption); // get the decrypted plaintext
    simpleserial_addcmd('c', 16, set_ciphertext);
    simpleserial_addcmd('l', 4, set_ciphertext_length); // in bits

	while(1)
		simpleserial_get();
}
