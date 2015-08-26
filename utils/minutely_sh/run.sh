#!/bin/bash

. list225
. listOther
list=(${list225[*]} ${listOther[*]})

for i in "${list[@]}"
do
    mkdir $i 2> /dev/null
    wget -q `date +"http://k-db.com/stocks/$i-T/minutely?date=%Y-%m-%d&download=csv" --date "1 day ago"` -Otmp
    grep `date +"%Y-%m-%d" --date "1 day ago"` tmp
    if [ $? -eq 1 ]
    then
        continue
    fi
    sed -e "1,2d" tmp > $i/`date +"%Y%m%d" --date "1 day ago"`
    sleep 1
done
