#! /bin/bash
#
# hardest uva problems
# Copyright (C) 2009 Diogo Sousa
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

#
# Usage ./problems_done.sh <user_id>

if [ "$#" == "0" ] || [ "$1" == '' ]; then
    echo 'Please give me at least one user id'
    exit -1
fi

if [ "$#" -gt "10" ]; then
    echo 'Too many user ids'
    exit -1
fi

for userid in $@; do
    orig=$userid
    tmp=`pcregrep -i "^$userid[ \t]+[0-9]+$" userids`

    if [ "$?" == "0" ]; then
	userid=`awk '{print $2}' <<< $tmp`
    fi

    egrep '^[0-9]+$' <<< $userid > /dev/null 2> /dev/null

    if [ "$?" != "0" ]; then
	echo "$orig: invalid user id"
	continue
    fi

    outraw=`curl "http://uva.onlinejudge.org/index.php?option=onlinejudge&page=show_authorstats&userid=$userid" 2> /dev/null`

    out=`echo "$outraw" | grep 'problem=' | tr '>' ' ' | tr '<' ' '`

    c=0
    for i in $out; do
        echo $i | egrep 'problem=[0-9]+' > /dev/null
	
        if [ $? = 0 ]; then
	    let c=$c+1            
        fi
    done

    if [ "$orig" == "$userid" ]; then
	printf "User id $userid: $c "
    else
	printf "User id $userid ($orig): $c "
    fi

    if [ "$c" == "1" ]; then
	echo "problem done"
    else
	echo "problems done"
    fi
done

exit 0
