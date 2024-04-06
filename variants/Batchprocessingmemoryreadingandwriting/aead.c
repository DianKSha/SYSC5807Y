// Reduce the number of memory visits: By processing data blocks in batches, the number of reading and writing memory is reduced, especially for the encryption and decryption operations of a large amount of data, which can significantly improve performance.

// Improve cache utilization: Batch processing of data blocks makes the data access mode more continuous, which is conducive to improving the utilization rate of CPU cache and reducing cache failure.

#include "api.h"
#include "ascon.h"
#include "crypto_aead.h"
#include "permutations.h"
#include "printstate.h"
#include "word.h"

// New auxiliary function: batch processing of data blocks
static void process_data_block(ascon_state_t* s, const unsigned char** data, unsigned long long* len, unsigned char** out, void (*perm_func)(ascon_state_t*)) {
    while (*len >= ASCON_128_RATE) {
        s->x[0] ^= LOADBYTES(*data, 8);
        if (out) {
            STOREBYTES(*out, s->x[0], 8);
            *out += ASCON_128_RATE;
        }
        perm_func(s);
        *data += ASCON_128_RATE;
        *len -= ASCON_128_RATE;
    }
    // Process the last data block
    if (*len > 0) {
        s->x[0] ^= LOADBYTES(*data, *len) ^ PAD(*len);
        if (out) {
            STOREBYTES(*out, s->x[0], *len);
            *out += *len;
        }
        perm_func(s);
    }
}


int crypto_aead_encrypt(unsigned char* c, unsigned long long* clen,
                        const unsigned char* m, unsigned long long mlen,
                        const unsigned char* ad, unsigned long long adlen,
                        const unsigned char* nsec, const unsigned char* npub,
                        const unsigned char* k) {
  (void)nsec;

  /* set ciphertext size */
  *clen = mlen + CRYPTO_ABYTES;

  /* load key and nonce */
  const uint64_t K0 = LOADBYTES(k + 0, 4) >> 32;
  const uint64_t K1 = LOADBYTES(k + 4, 8);
  const uint64_t K2 = LOADBYTES(k + 12, 8);
  const uint64_t N0 = LOADBYTES(npub, 8);
  const uint64_t N1 = LOADBYTES(npub + 8, 8);

  /* initialize */
  ascon_state_t s;
  s.x[0] = ASCON_80PQ_IV | K0;
  s.x[1] = K1;
  s.x[2] = K2;
  s.x[3] = N0;
  s.x[4] = N1;
  printstate("init 1st key xor", &s);
  P12(&s);
  s.x[2] ^= K0;
  s.x[3] ^= K1;
  s.x[4] ^= K2;
  printstate("init 2nd key xor", &s);

// Processing Associated Data (AD)
  if (adlen) {
      process_data_block(&s, &ad, &adlen, NULL, P6);
      s.x[4] ^= 1; // Domain separation
  }

  // Process plaintext data
  process_data_block(&s, &m, &mlen, &c, P6);

  /* domain separation */
  s.x[4] ^= 1;
  printstate("domain separation", &s);

  printstate("pad plaintext", &s);

  /* finalize */
  s.x[1] ^= K0 << 32 | K1 >> 32;
  s.x[2] ^= K1 << 32 | K2 >> 32;
  s.x[3] ^= K2 << 32;
  printstate("final 1st key xor", &s);
  P12(&s);
  s.x[3] ^= K1;
  s.x[4] ^= K2;
  printstate("final 2nd key xor", &s);

  /* set tag */
  STOREBYTES(c, s.x[3], 8);
  STOREBYTES(c + 8, s.x[4], 8);

  return 0;
}

int crypto_aead_decrypt(unsigned char* m, unsigned long long* mlen,
                        unsigned char* nsec, const unsigned char* c,
                        unsigned long long clen, const unsigned char* ad,
                        unsigned long long adlen, const unsigned char* npub,
                        const unsigned char* k) {
  (void)nsec;

  if (clen < CRYPTO_ABYTES) return -1;

  /* set plaintext size */
  *mlen = clen - CRYPTO_ABYTES;

  /* load key and nonce */
  const uint64_t K0 = LOADBYTES(k + 0, 4) >> 32;
  const uint64_t K1 = LOADBYTES(k + 4, 8);
  const uint64_t K2 = LOADBYTES(k + 12, 8);
  const uint64_t N0 = LOADBYTES(npub, 8);
  const uint64_t N1 = LOADBYTES(npub + 8, 8);

  /* initialize */
  ascon_state_t s;
  s.x[0] = ASCON_80PQ_IV | K0;
  s.x[1] = K1;
  s.x[2] = K2;
  s.x[3] = N0;
  s.x[4] = N1;
  printstate("init 1st key xor", &s);
  P12(&s);
  s.x[2] ^= K0;
  s.x[3] ^= K1;
  s.x[4] ^= K2;
  printstate("init 2nd key xor", &s);

// Processing Associated Data (AD)
if (adlen) {
    process_data_block(&s, &ad, &adlen, NULL, P6);
    s.x[4] ^= 1; // Domain separation
}

// Process ciphertext data
process_data_block(&s, &c, &clen, &m, P6);

  /* domain separation */
  s.x[4] ^= 1;
  printstate("domain separation", &s);

  /* full ciphertext blocks */
  clen -= CRYPTO_ABYTES;
  printstate("pad ciphertext", &s);

  /* finalize */
  s.x[1] ^= K0 << 32 | K1 >> 32;
  s.x[2] ^= K1 << 32 | K2 >> 32;
  s.x[3] ^= K2 << 32;
  printstate("final 1st key xor", &s);
  P12(&s);
  s.x[3] ^= K1;
  s.x[4] ^= K2;
  printstate("final 2nd key xor", &s);

  /* set tag */
  uint8_t t[16];
  STOREBYTES(t, s.x[3], 8);
  STOREBYTES(t + 8, s.x[4], 8);

  /* verify tag (should be constant time, check compiler output) */
  int i;
  int result = 0;
  for (i = 0; i < CRYPTO_ABYTES; ++i) result |= c[i] ^ t[i];
  result = (((result - 1) >> 8) & 1) - 1;

  return result;
}
