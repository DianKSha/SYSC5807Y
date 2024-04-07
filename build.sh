#! /bin/zsh

ASCON_BUILD_PY="ascon-build.py"
ASCON_BUILD_BASE_PY="ascon-build-base.py"
ASCON_TEST_BASE_PY="ascon-test-base.py"
ASCON_TEST_PY="ascon-test.py"
KAT_FILE="KAT.txt"
BASE_DIR="out/"
function help(){
    echo "test all variants against KAT.txt"
    echo "put the variant under variants/variant-name"
}

# only do when there is BASE_DIR
if [[ ! -d $BASE_DIR ]] ;
then
    echo "no $BASE_DIR "
    exit(1)
fi

echo "find variants ..."
for variant in $BASE_DIR/*;
do
    if [[ ! -d $variant ]];
    then
        continue;
    fi
    echo "==================START============="
    
    echo "==============FILE:$variant========="

    # copy the base build py to each variant
    cp $ASCON_BUILD_BASE_PY $variant/$ASCON_BUILD_PY
    cp $ASCON_TEST_BASE_PY $variant/$ASCON_TEST_PY
    cp $KAT_FILE $variant/$KAT_FILE
    CURRENT_DIR=$(pwd)
    
    cd $variant
    python3 $ASCON_BUILD_PY 
    cd $CURRENT_DIR

done


