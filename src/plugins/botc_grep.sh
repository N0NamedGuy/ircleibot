#!/bin/bash
# Acknoledgement of this code goes to orium

if [ "$1" == "" ]; then
    echo 'Greping for game'
    exp='game'
else
    exp=$1
fi

out=$(pcregrep -i "$exp" chatlog.txt)

if [ $? == 0 ]; then
    tail -n 1 <<< "$out"
else
    echo "No match for \"$exp\""
fi

exit 0
