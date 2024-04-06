
from ctypes import * 
so_file = "variants/sample_change/aead.so"


functions = CDLL(so_file)
