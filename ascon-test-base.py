import re

from _pi_ascon import ffi, lib
import binascii


countReg="^Count = (.*)$"
NonceReg= "^Nonce = (.*)$"
PTReg="^PT = (.*)$"
ADReg="^AD = (.*)$"
KeyReg="^Key = (.*)$"

CTReg="^CT = (.*)$"

class testVec:
    def __init__(self):
        count = 0
        key = 0
        plaintext = 0
        associatedData = 0
        ciphertext = 0
        nonce = 0

tests = []
with open("KAT.txt","r") as kat:
    test = testVec()
    for line in kat:

        if line == "\n":
            tests.append(test)
            test=testVec()
        t = re.match(countReg, line)
        if t is not None:
            test.count = int(t.group(1))
        t = re.match(KeyReg, line)
        if t is not None:
            test.key=t.group(1)
        t = re.match(NonceReg, line)
        if t is not None:
            test.nonce=t.group(1)

        t = re.match(PTReg, line)
        if t is not None:
            test.plaintext=t.group(1)

        t = re.match(ADReg, line)
        if t is not None:
            test.associatedData=t.group(1)
        t = re.match(CTReg, line)
        if t is not None:
            test.ciphertext=t.group(1)




print("======================TEST======================")

# test encryption

for test in tests:


    clen = ffi.new("unsigned long long*")
    m = binascii.unhexlify(test.plaintext)
    mlen = len(m)
    m = ffi.new(f"const unsigned char m[{mlen}]",m)
    c = ffi.new(f"unsigned char[{mlen+16}]")

    ad = binascii.unhexlify(test.associatedData)

    adlen=len(ad)

    ad = ffi.new(f"const unsigned char[{adlen}]", ad)
    nsec = ffi.new("unsigned char*")

    npub = binascii.unhexlify(test.nonce)

    npub = ffi.new("const unsigned char[16]", npub)


    key = binascii.unhexlify(test.key)
    key = ffi.new("const unsigned char[20]",  key)

    lib.crypto_aead_encrypt(c, clen, m, mlen, ad, adlen, nsec, npub, key)

    result = ""
    assert(clen[0] == 16+mlen) # test length
    clen = clen[0]
    for i in range(clen):
        temp = hex(c[i])[2:]
        if len(temp) == 1:
            temp = '0'+temp
        result += temp.upper()

    if result!=test.ciphertext:
        print("Unequal test during encryption:")
        print("from execution:")
        print(test.count)
        print(result)
        print("expected ciphertext:")
        print(test.ciphertext)
    # test decryption
    # decrypt the returned ciphertext


    c = binascii.unhexlify(test.ciphertext)
    clen = len(c)
    c = ffi.new(f"const unsigned char[{clen}]", c)
    mlen = ffi.new("unsigned long long *")
    m = ffi.new(f"unsigned char [{clen-16}]")
    lib.crypto_aead_decrypt(m,mlen,nsec,c,clen,ad,adlen,npub,key)

    assert(mlen[0] == clen-16)

    mlen = mlen[0]
    result = ""
    for t in range(mlen):
        temp = hex(m[ t])[2:]
        if len(temp) == 1:
            temp = '0' + temp
        result += temp.upper()

    if result!=test.plaintext:
        print("Unequal test during decryption:")
        print(test.count)
        print(result)
        print("expected plaintext:")
        print(test.plaintext)

# test decryption
