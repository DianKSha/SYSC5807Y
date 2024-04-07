#! /bin/zsh

ASCON_BUILD_PY="ascon-build.py"
ASCON_BUILD_BASE_PY="ascon-build-base.py"
ASCON_TEST_BASE_PY="ascon-test-base.py"
ASCON_TEST_PY="ascon-test.py"
KAT_FILE="KAT.txt"
BASE_DIR="out/"

# only do when there is BASE_DIR
if [[ ! -d $BASE_DIR ]] ;
then
    echo "no $BASE_DIR "
    exit(1)
fi

echo "TEST BEGIN:======================"
echo "find variants ..."
for variant in $BASE_DIR/*;
do
    if [[ ! -d $variant ]];
    then
        continue;
    fi
    echo "==================START============="
    
    echo "==============FILE:$variant========="

        
    if [[ $variant =~ '.*/#.*' ]];
    then
        echo "skip variant $variant"
        continue;
    fi


    # copy the base build py to each variant
    CURRENT_DIR=$(pwd)
    
    cd $variant
    python3 $ASCON_TEST_PY  >> testResultFile.txt
    cd $CURRENT_DIR

done


