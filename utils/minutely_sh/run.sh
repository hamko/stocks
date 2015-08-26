#!/bin/bash
. list225
. listOther
list=(${list225[*]} ${listOther[*]})

for i in "${list[@]}"
do
    mkdir $i 2> /dev/null
    wget -q `date +"http://k-db.com/stocks/$i-T/minutely?date=%Y-%m-%d&download=csv"` -Otmp
    grep `date +"%Y-%m-%d"` tmp
    if [ $? -eq 1 ]
    then
        continue
    fi
    sed -e "1,2d" tmp > $i/`date +"%Y%m%d"`
    sleep 1
done
