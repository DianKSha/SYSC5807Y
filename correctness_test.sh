#! /bin/zsh

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
done

