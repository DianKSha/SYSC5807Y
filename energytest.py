import ctypes
import os

# Assuming the shared library is named 'libmyaead.so' or 'libmyaead.dll' depending on the platform
lib_name = "libmyaead.so"
lib_path = os.path.join("/Users/jiongjieliu/Documents/Meng/2024Winter/Cryptographic Implementations and Side-Channel Analysis/Project/ascon-c-main/crypto_aead/ascon80pqv12/ref", "libmyaead.so")

# Load the shared library
my_aead_lib = ctypes.CDLL(lib_path)

# Define Python wrappers for the C functions based on `my_aead_module.h`
class AEADModule:
    def __init__(self, lib):
        self.lib = lib

        # Original Encrypt Function
        self.original_encrypt = lib.original_encrypt
        self.original_encrypt.restype = None
        self.original_encrypt.argtypes = [
            ctypes.c_char_p, ctypes.c_size_t,
            ctypes.c_char_p, ctypes.c_size_t,
            ctypes.c_char_p, ctypes.c_size_t,
            ctypes.c_char_p, ctypes.c_size_t,
            ctypes.POINTER(ctypes.c_char_p), ctypes.POINTER(ctypes.c_char_p)
        ]

        # Original Decrypt Function
        self.original_decrypt = lib.original_decrypt
        self.original_decrypt.restype = ctypes.c_int
        self.original_decrypt.argtypes = [
            ctypes.c_char_p, ctypes.c_size_t,
            ctypes.c_char_p, ctypes.c_size_t,
            ctypes.c_char_p, ctypes.c_size_t,
            ctypes.c_char_p, ctypes.c_size_t,
            ctypes.POINTER(ctypes.c_char_p), ctypes.POINTER(ctypes.c_size_t)
        ]

    def encrypt(self, key, nonce, plaintext, ad):
        # Placeholder for actual encryption logic
        ciphertext = ctypes.c_char_p()
        tag = ctypes.c_char_p()
        self.original_encrypt(
            key, len(key),
            nonce, len(nonce),
            plaintext, len(plaintext),
            ad, len(ad),
            ctypes.byref(ciphertext), ctypes.byref(tag)
        )
        return ciphertext.value, tag.value

    def decrypt(self, key, nonce, ciphertext, ad):
        # Placeholder for actual decryption logic
        plaintext = ctypes.c_char_p()
        plaintext_len = ctypes.c_size_t()
        result = self.original_decrypt(
            key, len(key),
            nonce, len(nonce),
            ciphertext, len(ciphertext),
            ad, len(ad),
            ctypes.byref(plaintext), ctypes.byref(plaintext_len)
        )
        if result == 0:
            return plaintext.value[:plaintext_len.value]
        else:
            return None

# Example usage
if __name__ == "__main__":
    aead = AEADModule(my_aead_lib)
    key = b"examplekey123456"
    nonce = b"nonce123"
    plaintext = b"secret message"
    ad = b"associated data"

    # Perform encryption
    ciphertext, tag = aead.encrypt(key, nonce, plaintext, ad)
    print(f"Ciphertext: {ciphertext}, Tag: {tag}")

    # Perform decryption
    decrypted = aead.decrypt(key, nonce, ciphertext, ad)
    print(f"Decrypted: {decrypted}")
