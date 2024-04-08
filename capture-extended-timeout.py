print('start')
trace_array = []   # Array placeholder for the traces
textin_array = []  # Array placeholder for the list of Plaintexts associated with each trace.
cipherTextArray=[]
solvPTArray=[]

traceArrayDe = []

key = bytearray([4]*20) # 20 bytes

plainText=bytearray([0]*16)
plaintextLengthBytes = bytearray([0,0,0,16])  # 16 byte
plaintextLength = 16 # bytes

associatedData = bytearray([6]*16)
associatedDataLengthBytes =  bytearray([0,0,0,16]) # four bytes
associatedDataLength = 16

ciphertext = None
ciphertextLength = 16
ciphertextLengthBytes = bytearray([0,0,0,16])

nIteration=10
repetitionNum = math.ceil(plaintextLength/16)

nonce = bytearray([7]*16)#bytes 32
nullByteArray=bytearray()

print('encrypt')
for i in range(nIteration):
    print(str(i)+'-encrypt')
    target.simpleserial_write('b', associatedDataLengthBytes, timeout=1000)
    target.simpleserial_read('r',4, timeout=1000)

    for t in range(math.ceil(associatedDataLength/16)):
        target.simpleserial_write('a',associatedData, timeout=1000)
        target.simpleserial_read('r',  16, timeout=1000)
    target.simpleserial_write('n', nonce, timeout=1000)
    target.simpleserial_read('r',  16, timeout=1000)

    target.simpleserial_write('k', key, timeout=1000)
    target.simpleserial_read('r',  20, timeout=1000)

    target.simpleserial_write('q',plaintextLengthBytes, timeout=1000)
    target.simpleserial_read('r',4, timeout=1000)
    for i in range(repetitionNum):
        target.simpleserial_write('p',plainText, timeout=1000)
        target.simpleserial_read('r',  16, timeout=1000)
    
    scope.arm() # enable capturing
    target.simpleserial_write('e', nullByteArray, timeout=1000)
    ret = scope.capture()
    if ret:
        print("Target timed out!")
        print(f"The intput = {plainText} and key is {key}")
        continue
    
    trace_array.append(scope.get_last_trace()) # append the power trace
    
    res_ciphertext = target.simpleserial_read('r',16, timeout=1000)
    
    ciphertext=res_ciphertext
    
    textin_array.append(plainText) # append the plaintext of this run
    cipherTextArray.append(res_ciphertext)
    plainText=res_ciphertext[1:]
    plainText+=bytearray([1])
    time.sleep(0.05)
    
    print(str(i)+'-decrypt')
    print(ciphertext)
    target.simpleserial_write('b', associatedDataLengthBytes, timeout=1000)
    #target.simpleserial_read('r',  4, timeout=1000)
    for t in range(math.ceil(associatedDataLength/16)):
        target.simpleserial_write('a',associatedData, timeout=1000)
        #target.simpleserial_read('r',  16, timeout=1000)

    target.simpleserial_write('n', nonce, timeout=1000)
    #target.simpleserial_read('r', 16, timeout=1000)
    target.simpleserial_write('k', key, timeout=1000)
    #target.simpleserial_read('r', 20, timeout=1000)
    target.simpleserial_write('l',ciphertextLengthBytes, timeout=1000)
    #target.simpleserial_read('r',  4, timeout=1000)
    for i in range(repetitionNum):
        target.simpleserial_write('c', ciphertext, timeout=1000)
        #target.simpleserial_read('r',  16, timeout=1000)

    res_plaintext = bytearray()
    
    scope.arm() # enable capturing
    target.simpleserial_write('d', nullByteArray, timeout=1000)
    ret = scope.capture()
    if ret:
        
        print("Target timed out!")
        print(f"The intput_ciphertext = {ciphertext} and key is {key}")
        continue
    traceArrayDe.append(scope.get_last_trace()) # append the power trace
    res_plaintext= target.simpleserial_read('r',16, timeout=1000)
    solvPTArray.append(res_plaintext)
    
    time.sleep(0.05)

