## SYSC5807 Cryptography Project

Goal : Minimize energy consumption



For every variant (a versoin of change), 
copy the simple-serial directory and then change the corresponding folder

put the changed directory into variants/ directory


COPY THE SIMPLESERIAL FOLDER AND THEN CHANGE, COPY THE FOLDER FIRST

```
cp -r simpleserial-ascon80pq variants/<VARIANT NAME>
```
then CHANGE, DO NOT DELETE FILES, DO NOT DELETE FILES, DO NOT DELETE FILES, 

### to test

run `make`, there would be an `out` directory.

The `out` contains a copy of each variant under `variants`, the test result is put in a file called `testResultFile.txt` under `out/<variant-name>`.

The file contains incorrect results against KAT. A correct program should only display a line 

IF there is any erro with makefile itself, most likely it's  from the  PYTHON and PIP variable, change to corresponding one on your machine. note that it's mainly for Linux system.

```

=========TEST=============
```

PLEASE MAKE SURE THE CHANGED CODE CAN BE COMPILED WITHOUT ERRORS (AND WARNINGS  if warnings are really ignorable)


The codes can not be compiled, they all  lack some files. Make sure first copy the source folder, then change


I put lacked files on my machine, NONE OF THEM ARE CORRECT. DO NOT LET  GPT WRITE CODES. I don't think it can write these codes.

To try if it can compile, just run `make` in the root directory. then it will compile all variants, just find the one you are trying.
Look for `=============================` and adjacent lines for information


I've copied a example variant, called `sample_change`, it can run without errors and warnings. It just contains the reference implementation.




If you are stuck on something you've never heard of, go write the paper :) also our data needs visualization, you could definitely help with that, e.g. draw histograms with data, etc.
