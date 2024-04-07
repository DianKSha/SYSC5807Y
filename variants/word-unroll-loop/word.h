#ifndef WORD_H_
#define WORD_H_

#include <stdint.h>
#include <string.h>

typedef uint64_t uint64_t;

/* get byte from 64-bit Ascon word */
#define GETBYTE(x, i) ((uint8_t)((uint64_t)(x) >> (56 - 8 * (i))))

/* set byte in 64-bit Ascon word */
#define SETBYTE(b, i) ((uint64_t)(b) << (56 - 8 * (i)))

/* set padding byte in 64-bit Ascon word */
#define PAD(i) SETBYTE(0x80, i)


// =====optimization======
// load 8 bytes, get rid of counter overhead
// unroll the loop
static inline uint64_t LOAD8BYTES(const uint8_t* bytes){
    uint64_t x = 0;
    x |= (uint64_t)bytes[0]; x=x<<8;
    x |= (uint64_t)bytes[1]; x=x<<8;
    x |= (uint64_t)bytes[2]; x=x<<8;
    x |= (uint64_t)bytes[3]; x=x<<8;
    x |= (uint64_t)bytes[4]; x=x<<8;
    x |= (uint64_t)bytes[5]; x=x<<8;
    x |= (uint64_t)bytes[6]; x=x<<8;
    x |= (uint64_t)bytes[7];
    return x;
}
// =====optimization======
// load  4 bytes, get rid of counter overhead
// unroll the loop
// load 0000xxxx
static inline uint64_t  LOADLOWER4BYTES(const uint8_t* bytes){
    uint64_t x = 0;
    x |= (uint64_t)bytes[0]; x=x<<8;
    x |= (uint64_t)bytes[1]; x=x<<8;
    x |= (uint64_t)bytes[2]; x=x<<8;
    x |= (uint64_t)bytes[3];
    return x;
}
// =====optimization======
// load  4 bytes, get rid of counter overhead
// unroll the loop
// load xxxx0000
static inline uint64_t  LOADHIGHER4BYTES(const uint8_t* bytes){
    uint64_t x = 0;
    x |= (uint64_t)bytes[0]; x=x<<8; //0000_000x
    x |= (uint64_t)bytes[1]; x=x<<8; //0000_00xx
    x |= (uint64_t)bytes[2]; x=x<<8; //0000_0xxx
    x |= (uint64_t)bytes[3]; //0000_xxxx
    x=x<<32;//0000_xxxx->xxxx_0000
    return x;
}

// 
// =====optimization======
// store 8 bytes into bytes
static inline void STORE8BYTES(uint8_t* bytes, uint64_t x) {
  bytes[7] = (uint8_t)x; x=x>>8;
  bytes[6] = (uint8_t)x; x=x>>8;
  bytes[5] = (uint8_t)x; x=x>>8;
  bytes[4] = (uint8_t)x; x=x>>8;
  bytes[3] = (uint8_t)x; x=x>>8;
  bytes[2] = (uint8_t)x; x=x>>8;
  bytes[1] = (uint8_t)x; x=x>>8;
  bytes[0] = (uint8_t)x; 
}


/* load bytes into 64-bit Ascon word */
static inline uint64_t LOADBYTES(const uint8_t* bytes, int n) {
  int i;
  uint64_t x = 0;
  for (i = 0; i < n; ++i) x |= SETBYTE(bytes[i], i);
  return x;
}

/* store bytes from 64-bit Ascon word */
static inline void STOREBYTES(uint8_t* bytes, uint64_t x, int n) {
  int i;
  for (i = 0; i < n; ++i) bytes[i] = GETBYTE(x, i);
}

/* clear bytes in 64-bit Ascon word */
static inline uint64_t CLEARBYTES(uint64_t x, int n) {
  int i;
  for (i = 0; i < n; ++i) x &= ~SETBYTE(0xff, i);
  return x;
}

#endif /* WORD_H_ */
