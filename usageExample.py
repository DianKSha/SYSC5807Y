
import math

key = bytearray([0]*20) # 20 bytes

plaintext = bytearray([1]*16) #  16 bytes of palintext
plaintextLengthBytes = bytearray([0,0,0,16])  # 16 byte
plaintextLength = 16 # bytes
repetitionNum = math.floor(plaintextLength/16)

associatedData = bytearray([6]*16)
associatedDataLengthBytes =  bytearray([0,0,0,16]) # four bytes

ciphertext = None
ciphertextLength = 16
ciphertextLengthBytes = bytearray([0,0,0,16])

nonce = bytearray([1,2]*16)#bytes 32
# to encrypt

target.simpleserial_write('b', associatedDataLengthBytes)
target.simpleserial_write('a',associatedData)
target.simpleserial_write('n', nonce)



target.simpleserial_write('k', key)

target.simpleserial_write('q',plaintextLengthBytes)
for i in range(repetitionNum):
    target.simpleserial_write('p',plaintext)

res_ciphertext = ''
for i in range(repetitionNum):
    target.simpleserial_write('e', bytearray())
    res_ciphertext +=   target.simpleserial_read('r',16)


# for testing
# change ciphertext
# should be available before 

ciphertext = bytearray(res_ciphertext, 'ascii')
# ============= 
# to decrypt


target.simpleserial_write('b', associatedDataLengthBytes)
target.simpleserial_write('a',associatedData)
target.simpleserial_write('n', nonce)


target.simpleserial_write('k', key)

target.simpleserial_write('l',ciphertextLengthBytes)
for i in range(repetitionNum):

    target.simpleserial_write('c', ciphertext)

res_plaintext = ''
for i in range(repetitionNum):
    target.simpleserial_write('d', bytearray())
    res_ciphertext +=   target.simpleserial_read('r',16)


# print the plaintext and recovered palintext

print(plaintext)
print(res_plaintext)







