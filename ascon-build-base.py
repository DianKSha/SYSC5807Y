from cffi import FFI

ffibuilder = FFI()

ffibuilder.cdef("""
int crypto_aead_encrypt(unsigned char* c, unsigned long long* clen,
                        const unsigned char* m, unsigned long long mlen,
                        const unsigned char* ad, unsigned long long adlen,
                        const unsigned char* nsec, const unsigned char* npub,
                        const unsigned char* k);
int crypto_aead_decrypt(unsigned char* m, unsigned long long* mlen,
                        unsigned char* nsec, const unsigned char* c,
                        unsigned long long clen, const unsigned char* ad,
                        unsigned long long adlen, const unsigned char* npub,
                        const unsigned char* k) ;
""")

ffibuilder.set_source("_pi_ascon",
                      """
                      #include "crypto_aead.h"
                      """,

                      sources=["ascon-ref/aead.c"],


                      include_dirs=["./ascon-ref"],

                      library_dirs=["./ascon-ref"],
                      libraries=[])
if __name__ == "__main__":
    ffibuilder.compile(verbose=True)
