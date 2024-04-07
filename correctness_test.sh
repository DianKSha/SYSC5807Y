#! /bin/zsh

ASCON_BUILD_PY="ascon-build.py"
ASCON_BUILD_BASE_PY="ascon-build-base.py"
function help(){
    echo "test all variants against KAT.txt"
    echo "put the variant under variants/variant-name"
}

for variant in variants/*;
do
    echo "find variants ..."
    if [[ -d $variant ]];
    then
        echo $variant
    fi
    # copy the base
    cp ASCON_BUILD_BASE_PY $variant/$ASCON_BUILD_PY
     echo "ffibuilder.set_source(\"_pi_ascon\",\"\"\"\n\t#include \"crypto_aead.h\"\n\t\"\"\",sources=[\"$variant/aead.c\"],include-dirs=[\"xxxx\"],libraries=[])" >>   $variant/$ASCON_BUILD_PY


done


 echo "ffibuilder.set_source(\"_pi_ascon\",\"\"\"\n\t#include \"crypto_aead.h\"\n\t\"\"\",sources=[\"xxx\"],include-dirs=[\"xxxx\"],libraries=[])"
