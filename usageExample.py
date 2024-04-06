
import math

key = bytearray([0]*20) # 20 bytes

plaintext = bytearray([1,2]*13) #  16 bytes of palintext
plaintextLengthBytes = bytearray([0,0,0,26])  # 16 byte
plaintextLength = 26 # bytes
repetitionNum = math.ceil(plaintextLength/16)

associatedData = bytearray([1,2,3]*12)
associatedDataLengthBytes =  bytearray([0,0,0,36]) # four bytes
associatedDataLength = 36

ciphertext = None
ciphertextLength = plaintextLength
ciphertextLengthBytes = plaintextLengthBytes


nonce = bytearray([7]*16)#bytes 16
# to encrypt

target.simpleserial_write('b', associatedDataLengthBytes)
print(target.simpleserial_read('r',4))

for t in range(math.ceil(associatedDataLength/16)):

    target.simpleserial_write('a',associatedData)
    print(target.simpleserial_read('r',  16))
target.simpleserial_write('n', nonce)
print(target.simpleserial_read('r',  16))


target.simpleserial_write('k', key)
print(target.simpleserial_read('r',  20))

target.simpleserial_write('q',plaintextLengthBytes)
print(target.simpleserial_read('r',4))
for i in range(repetitionNum):
    target.simpleserial_write('p',plaintext)
    print(target.simpleserial_read('r',  16))

res_ciphertext = ''
for i in range(repetitionNum):
    target.simpleserial_write('e', bytearray())
    temp = target.simpleserial_read('r',16)
    print(temp)
    res_ciphertext += temp


# for testing
# change ciphertext
# should be available before 

ciphertext = bytearray(res_ciphertext, 'ascii')
# ============= 
# to decrypt


target.simpleserial_write('b', associatedDataLengthBytes)
print(target.simpleserial_read('r',  4))
for t in range(math.ceil(associatedDataLength/16)):
    target.simpleserial_write('a',associatedData)
    print(target.simpleserial_read('r',  16))

target.simpleserial_write('n', nonce)
print(target.simpleserial_read('r',   16))
target.simpleserial_write('k', key)
print(target.simpleserial_read('r',     20))

target.simpleserial_write('l',ciphertextLengthBytes)
print(target.simpleserial_read('r',  4))
for i in range(repetitionNum):

    target.simpleserial_write('c', ciphertext)
    print(target.simpleserial_read('r',  16))

res_plaintext = ''
for i in range(repetitionNum):
    target.simpleserial_write('d', bytearray())
    temp = target.simpleserial_read('r',16)
    print(temp)
    
    res_plaintext+= temp



# print the plaintext and recovered palintext

print(plaintext)
print(res_plaintext)







