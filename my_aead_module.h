
#ifndef MY_AEAD_MODULE_H
#define MY_AEAD_MODULE_H

#include <stddef.h> // For size_t

void original_encrypt(const unsigned char *key, size_t key_len,
                      const unsigned char *nonce, size_t nonce_len,
                      const unsigned char *plaintext, size_t pt_len,
                      const unsigned char *ad, size_t ad_len,
                      unsigned char **ciphertext, unsigned char **tag);

void modified_encrypt(const unsigned char *key, size_t key_len,
                      const unsigned char *nonce, size_t nonce_len,
                      const unsigned char *plaintext, size_t pt_len,
                      const unsigned char *ad, size_t ad_len,
                      unsigned char **ciphertext, unsigned char **tag);

int original_decrypt(const unsigned char *key, size_t key_len,
                     const unsigned char *nonce, size_t nonce_len,
                     const unsigned char *ciphertext, size_t ct_len,
                     const unsigned char *ad, size_t ad_len,
                     unsigned char **plaintext, size_t *pt_len);

int modified_decrypt(const unsigned char *key, size_t key_len,
                     const unsigned char *nonce, size_t nonce_len,
                     const unsigned char *ciphertext, size_t ct_len,
                     const unsigned char *ad, size_t ad_len,
                     unsigned char **plaintext, size_t *pt_len);

#endif // MY_AEAD_MODULE_H
