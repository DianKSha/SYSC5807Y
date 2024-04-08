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

// all in bytes
#define NONCE_BYTES 16
#define KEY_BYTES 20
#define PLAINTEXT_BYTES 16
#define CIPHERTEXT_BYTES 32
#define TAG_BYTES 16
#define AD_BYTES 16

static unsigned char key[ KEY_BYTES] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19}; // 20 bytes key for ascon 80pq

static unsigned char associatedData[AD_BYTES] = {0};
static unsigned char plaintext[PLAINTEXT_BYTES] = {0};
static unsigned char ciphertext[CIPHERTEXT_BYTES] = {0};

static unsigned char nonce[NONCE_BYTES] = {0};

// hanlder
uint8_t  set_key(uint8_t* k, uint8_t len)
{
	// Load key here
    for (int i = 0; i<KEY_BYTES; i++){
        key[i] = *(k+i);
    }
    simpleserial_put('r',  KEY_BYTES,key);
    return 0x00;
}
uint8_t set_nonce(uint8_t* data, uint8_t len)
{
    // set the nonce
    for (int i = 0; i<NONCE_BYTES; i++){
        nonce[i] = *( data+i);
    }
    simpleserial_put('r', NONCE_BYTES , nonce);
	return 0x00;
}

uint8_t set_plaintext(uint8_t * data, uint8_t len){
    // set the plaintext 
    for (int i = 0; i<PLAINTEXT_BYTES; i++){
        plaintext[i] = *(data+i);
    }
    simpleserial_put('r', PLAINTEXT_BYTES, plaintext);
   return   0x00; 
}
uint8_t set_ciphertext(uint8_t * data, uint8_t len){
    // set the ciphertext , should include the tag as the last 16 bytes
    
    for(int i = 0; i<CIPHERTEXT_BYTES; i++){
        ciphertext[i] = *(data+i);
    }

    simpleserial_put('r', CIPHERTEXT_BYTES, ciphertext);
    return   0x00; 
}


uint8_t set_associated_data(uint8_t * data, uint8_t len){
    
    
    for (int i = 0; i < AD_BYTES; i++) {
        associatedData[i] = *(data+i);
    }
    simpleserial_put('r', AD_BYTES, associatedData);

    return   0x00; 
}


void encrypt(){
    unsigned long long clen;
    crypto_aead_encrypt(ciphertext, &clen, plaintext, PLAINTEXT_BYTES, associatedData, AD_BYTES, (void*)0, nonce, key);
}

int decrypt(){
    
    unsigned long long plen;
    return 
    crypto_aead_decrypt(plaintext, &plen, (void*)0, ciphertext, CIPHERTEXT_BYTES, associatedData, AD_BYTES,nonce, key);

}

uint8_t get_encryption(uint8_t* data, uint8_t len)
{

    trigger_high();
    encrypt();
    trigger_low();
    simpleserial_put('r', CIPHERTEXT_BYTES, ciphertext);
	return 0x00;
}
uint8_t get_decryption(uint8_t* data, uint8_t len){
    
    trigger_high();
    decrypt();
    trigger_low();
    simpleserial_put('r', PLAINTEXT_BYTES, plaintext);
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
	simpleserial_addcmd('k', KEY_BYTES,  set_key); // set key, 20 bytes
    simpleserial_addcmd('a', AD_BYTES, set_associated_data); // set associated data
    simpleserial_addcmd('n', NONCE_BYTES, set_nonce) ;
    simpleserial_addcmd('p', PLAINTEXT_BYTES, set_plaintext);
    simpleserial_addcmd('e', 16, get_encryption); // get the encrypted plaintext along with verification tag, 
    simpleserial_addcmd('d', 16, get_decryption); // get the decrypted plaintext
    simpleserial_addcmd('c', CIPHERTEXT_BYTES, set_ciphertext);

	while(1)
		simpleserial_get();
}
