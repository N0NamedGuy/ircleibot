#! /bin/bash

function fib
{
    n=$1
    last=1;
    current=0;
    
    for i in `seq 0 $n`; do
        let current=$last+$current;
        let last=$current-$last;
    done
    
    echo $current
}

if [ "$1" == "" ] || [ "$#" -le "1" ]; then
    echo "Sintaxe: !fibonacci <n> <string>"
    exit -1
fi

n=$1
str=$2

egrep '^[0-9]+$' <<< "$n" > /dev/null 2> /dev/null

if [ "$?" != "0" ]; then
    echo "$n: Invalid number"
    exit -1
fi

if [ "$n" -gt 6 ]; then
    echo "$n: F($n) is too big"
    exit -1
fi

for i in $(seq 0 $n); do
    for j in $(seq 1 $(fib $i)); do
	printf "$str "
    done
    echo
done

exit 0
