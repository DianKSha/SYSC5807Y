import random
def get_random_bytearray(n : int):
    # get a random btyearray of size n
    res = []
    for i in  range(n):
        res.append(random.randint(0,255)
    assert len(res) == n
    return bytearray(res)


trace_array = []
trace_decryption_array =  []
decrypted_plaintext_array = []

plaintext_array = []
ciphertext_array = []

nonce_array = [] # nonce need to be unique for every time of encryption
dummy = bytearray([0]*16)
keyLength = 20
nonceLength = 16
tagLength = 16
plaintextLength = 16
ciphertextLength =  plaintextLength + tagLength
associatedDataLength = 16

key = get_random_bytearray(keyLength)
associatedData = get_random_bytearray(associateDataLength)
plaintext = get_random_bytearray(plaintextLength)
ciphertext = get_random_bytearray(ciphertextLength)


sleepingTime = 0.1
nIteration=10
nullByteArray=bytearray()
print('encrypt')
for i in range(nIteration):

    print(str(i)+'-encrypt')
    # set random key, ad, pt, 
    key = get_random_bytearray(keyLength)
    associatedData = get_random_bytearray(associateDataLength)
    plaintext = get_random_bytearray(plaintextLength)

    nonce = get_random_bytearray(nonceLength)
    while(nonce in  nonce_array):
        nonce = get_random_bytearray(nonceLength) # ensure each encryption uses a unique nonce
    nonce_array.append(nonce)

    target.simpleserial_write('k', key)
    print("key sent back:")
    print(target.simleserial_read('r',keyLength)
    print(f"real key: {key}")
    target.simpleserial_write('a', associatedData)
    print("ad sent back:")
    print(target.sipmleserial_read('r',associatedDataLength, timeout=1000))
    print(f"real AD: {associatedData}")
    target.simpleserial_write('n',nonce)
    print("nonce sent back:")
    print(target.simpleserial_read('r', nonceLength))
    print(f"real nonce : {nonce}")
    target.simpleserial_write('p', plaintext)
    print("plaintext sent   back:")
    print(target.simpleserial_read('r',plaintextLength))
    print(f"real plaintext :{plaintext}")

    scope.arm() # enable capturing
    target.simpleserial_write('e',dummy) # do the encryption and put back ciphertext
    ret = scope.capture()
    if ret:
        print("Target timed out!")
        print(f"The intput = {plaintext} and key is {key}")
        continue
    
    trace_array.append(scope.get_last_trace()) # append the power trace
    ciphertext = target.simpleserial_read('r',ciphertextLength)
    print(f"ciphertext sent back after encryption :")
    print(ciphertext)

    
    textin_array.append(plaintext) # re cord plaintext
    ciphertext_array.append(ciphertext) # record ciphertext
    
    time.sleep( sleepingTime)
    
    print(str(i)+'-decrypt')
    assert type(ciphertext) == type(bytearray()) # must be an bytearray
    assert len(ciphertext) ==   ciphertextLength # length  must match
    print(ciphertext)


    # write key, nonce, ad, ciphertext
    target.simpleserial_write('k', key)
    print("key sent back:")
    print(target.simleserial_read('r',keyLength)
    print(f"real key: {key}")
    target.simpleserial_write('a', associatedData)
    print("ad sent back:")
    print(target.sipmleserial_read('r',associatedDataLength, timeout=1000))
    print(f"real AD: {associatedData}")
    target.simpleserial_write('n',nonce)
    print("nonce sent back:")
    print(target.simpleserial_read('r', nonceLength))
    print(f"real nonce : {nonce}")
    
    target.simpleserial_write('c', ciphertext)
    print("ciphertext sent   back:")
    print(target.simpleserial_read('r',ciphertextLength))
    print(f"real ciphertext :{ciphertext}")
    
    scope.arm() # enable capturing
    target.simpleserial_write('d', dummy) # decrypt
    ret = scope.capture()
    if ret:
        
        print("Target timed out!")
        print(f"The intput_ciphertext = {ciphertext} and key is {key}")
        continue

    trace_decryption_array.append(scope.get_last_trace()) # trace
    res_plaintext= target.simpleserial_read('r',plaintextLength)

    decrypted_plaintext_array.append(res_plaintext)
    print("decrypted plaintext:")
    print(res_plaintext)
    print("real plaintext:")
    print(plaintext)
    
    time.sleep ( sleepingTime)


