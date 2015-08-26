#!/bin/bash

. list225
. listOther
list=(${list225[*]} ${listOther[*]})
date_shift="1 day ago"

for i in "${list[@]}"
do
    mkdir $i 2> /dev/null
    wget -q `date +"http://k-db.com/stocks/$i-T/minutely?date=%Y-%m-%d&download=csv"  --date "$date_shift"` -Otmp
    grep `date +"%Y-%m-%d" --date "$date_shift"` tmp
    if [ $? -eq 1 ]
    then
        continue
    fi
    sed -e "1,2d" tmp > $i/`date +"%Y%m%d" --date "$date_shift"`
    sleep 1
done
