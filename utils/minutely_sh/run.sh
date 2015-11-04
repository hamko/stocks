#!/bin/bash

. list225
. listOther
list=(${list225[*]} ${listOther[*]})
date_shift="0 day ago"

for i in "${list[@]}"
do
    sleep 1
    echo $i

    mkdir $i 2> /dev/null
    wget -q `date +"http://k-db.com/stocks/$i-T/minutely?date=%Y-%m-%d&download=csv"  --date "$date_shift"` -Otmp

    fsize=`wc -c tmp | awk '{print $1}'`
    if [ $fsize -eq 0 ]
    then
        echo "rejected"
        continue
    fi

    grep `date +"%Y-%m-%d" --date "$date_shift"` tmp > /dev/null
    if [ $? -eq 1 ]
    then
        echo "no trade day"
        continue
    fi

    sed -e "1,2d" tmp > $i/`date +"%Y%m%d" --date "$date_shift"`
done
