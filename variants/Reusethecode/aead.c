//Simplified state initialization: By using the INIT_STATE macro, we can centrally process the initialization logic of the state in one place, reduce duplicate code and improve the readability and maintainability of the code.

//Reuse data absorption logic: Through the absorb_data function, we abstract the absorption process of associated data (AD) and plaintext data, avoiding the repetition of similar code in the encryption function, thus simplifying the implementation of encryption logic.

#include "api.h"
#include "ascon.h"
#include "crypto_aead.h"
#include "permutations.h"
#include "printstate.h"
#include "word.h"

#define INIT_STATE(s, k, npub) do { \
    s.x[0] = ASCON_80PQ_IV | LOADBYTES(k + 0, 4) >> 32; \
    s.x[1] = LOADBYTES(k + 4, 8); \
    s.x[2] = LOADBYTES(k + 12, 8); \
    s.x[3] = LOADBYTES(npub, 8); \
    s.x[4] = LOADBYTES(npub + 8, 8); \
} while(0)

void absorb_data(ascon_state_t* s, const unsigned char* data, unsigned long long len, void (*perm_func)(ascon_state_t*)) {
    while (len >= ASCON_128_RATE) {
        s->x[0] ^= LOADBYTES(data, 8);
        perm_func(s);
        data += ASCON_128_RATE;
        len -= ASCON_128_RATE;
    }
    s->x[0] ^= LOADBYTES(data, len);
    s->x[0] ^= PAD(len);
    perm_func(s);
}

int crypto_aead_encrypt(unsigned char* c, unsigned long long* clen,
                        const unsigned char* m, unsigned long long mlen,
                        const unsigned char* ad, unsigned long long adlen,
                        const unsigned char* nsec, const unsigned char* npub,
                        const unsigned char* k) {
    (void)nsec;
    *clen = mlen + CRYPTO_ABYTES;
    ascon_state_t s;

    // Initialize state
    INIT_STATE(s, k, npub);
    printstate("init 1st key xor", &s);
    P12(&s);
    s.x[2] ^= LOADBYTES(k + 0, 4) >> 32;
    s.x[3] ^= LOADBYTES(k + 4, 8);
    s.x[4] ^= LOADBYTES(k + 12, 8);
    printstate("init 2nd key xor", &s);

    // Absorb associated data
    if (adlen > 0) {
        absorb_data(&s, ad, adlen, P6);
    }
    s.x[4] ^= 1; // Domain separation
    printstate("domain separation", &s);

    // Absorb plaintext
    absorb_data(&s, m, mlen, P6);
    
    // Finalize
    s.x[1] ^= LOADBYTES(k + 0, 4) >> 32 << 32 | LOADBYTES(k + 4, 8) >> 32;
    s.x[2] ^= LOADBYTES(k + 4, 8) << 32 | LOADBYTES(k + 12, 8) >> 32;
    s.x[3] ^= LOADBYTES(k + 12, 8) << 32;
    printstate("final 1st key xor", &s);
    P12(&s);
    s.x[3] ^= LOADBYTES(k + 4, 8);
    s.x[4] ^= LOADBYTES(k + 12, 8);
    printstate("final 2nd key xor", &s);

    // Set tag
    STOREBYTES(c, s.x[3], 8);
    STOREBYTES(c + 8, s.x[4], 8);

    return 0;
}
