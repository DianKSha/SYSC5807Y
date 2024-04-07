
import math


def padByteArray(arr):
    # append 0 to arr to make it a multiple of sixteen
    if (len(arr)%16) == 0:
        return arr
    arr += bytearray([0])*(16-len(arr)%16)
    return arr


key = bytearray([0]*20) # 20 bytes

plaintext =padByteArray( bytearray([1,2]*13)) #  16 bytes of palintext
plaintextLengthBytes = bytearray([0,0,0,26])  # 16 byte
plaintextLength = 26 # bytes
repetitionNum = math.ceil(plaintextLength/16)

associatedData =  padByteArray(bytearray([1,2,3]*12))
associatedDataLengthBytes =  bytearray([0,0,0,36]) # four bytes
associatedDataLength = 36

ciphertext = None
ciphertextLength = plaintextLength+16 # plus tag
ciphertextLengthBytes =  bytearray([0,0,0,42])


nonce = bytearray([7]*16)#bytes 16
# to encrypt

target.simpleserial_write('b', associatedDataLengthBytes)
print(target.simpleserial_read('r',4))

for t in range(math.ceil(associatedDataLength/16)):

    target.simpleserial_write('a',associatedData[t*16: (t+1)*16])
    print(target.simpleserial_read('r',  16))
target.simpleserial_write('n', nonce)
print(target.simpleserial_read('r',  16))


target.simpleserial_write('k', key)
print(target.simpleserial_read('r',  20))

target.simpleserial_write('q',plaintextLengthBytes)
print(target.simpleserial_read('r',4))
for i in range(repetitionNum):
    target.simpleserial_write('p',plaintext[i*16:(i+1)*16])
    print(target.simpleserial_read('r',  16))

res_ciphertext = ''
for i in range(2*(repetitionNum+1)-1): # include tag
    target.simpleserial_write('e', bytearray())
    temp = str (target.simpleserial_read('r',16))
    print(temp)
    res_ciphertext += temp


# for testing
# change ciphertext
# should be available before 



ciphertext = padByteArray(bytearray(res_ciphertext, 'ascii'))
# ============= 
# to decrypt

key = bytearray([0]*20) # 20 bytes

plaintext =padByteArray( bytearray([1,2]*13)) #  16 bytes of palintext
plaintextLengthBytes = bytearray([0,0,0,26])  # 16 byte
plaintextLength = 26 # bytes
repetitionNum = math.ceil(plaintextLength/16)

associatedData =  padByteArray(bytearray([1,2,3]*12))
associatedDataLengthBytes =  bytearray([0,0,0,36]) # four bytes
associatedDataLength = 36

ciphertext = None
ciphertextLength = plaintextLength+16 # plus tag
ciphertextLengthBytes =  bytearray([0,0,0,42])


nonce = bytearray([7]*16)#bytes 16
#

target.simpleserial_write('b', associatedDataLengthBytes)
print(target.simpleserial_read('r',  4))
for t in range(math.ceil(associatedDataLength/16)):
    target.simpleserial_write('a',associatedData[t*16 : (t+1)*16])
    print(target.simpleserial_read('r',  16))

target.simpleserial_write('n', nonce)
print(target.simpleserial_read('r',   16))
target.simpleserial_write('k', key)
print(target.simpleserial_read('r',     20))

target.simpleserial_write('l',ciphertextLengthBytes)
print(target.simpleserial_read('r',  4))
for i in range(repetitionNum):

    target.simpleserial_write('c', ciphertext [i*16: (i+1)*16])
    print(target.simpleserial_read('r',  16))

res_plaintext = ''
for i in range(repetitionNum):
    target.simpleserial_write('d', bytearray())
    temp = str(target.simpleserial_read('r',16))
    print(temp)
    
    res_plaintext+= temp



# print the plaintext and recovered palintext

print(plaintext)
print(res_plaintext)







