## SYSC5807 Cryptography Project

Goal : Minimize energy consumption

### Introduction

- KAT.txt: the KAT provided in the ASCON's repo
- Makefile, build.sh, test.sh : used to test correctness of each variant
- variants : each variant is put in a folder under it
- ascon-ref : reference of ASCON80-pq
- simpleserial-ascon80pq : the template of running ascon on the  target, edited from simpleserial-base of  Chipwhisperer.

### To verify correctness

`make`, an `out` directory would be created and each variant under `variants` folder would be compiled and tested against KAT. 
This is implemented by calling C from python. Since we are compiling it with clang, it means anything  exclusive to arm compiler cannot be compiled and tested on our computers. Those variants  starts with `#` and are ignored.

Under each variant in the `out`, a `testResultFile.txt` would be generated to contain ERRORs.

If a variant passes all KAT, then it would only  contain a single line starting with `========`.

The tests are run on Macbook Pro, Intel CPU.

### To download to target

Copy each variant to Chipwhisperer's corresponding location. It needs chipwhisperer to compile and download





