#!/bin/bash

cp `date +"%Y%m%d" --date "1 day ago"` a
octave analize.m

cat M_Expensive.dat | mail -s `date +"%Y%m%d" --date "1 day ago"` wakataberyo@gmail.com
cat M_Economical.dat | mail -s `date +"%Y%m%d" --date "1 day ago"` wakataberyo@gmail.com
cat M_GoodCompany.dat | mail -s `date +"%Y%m%d" --date "1 day ago"` wakataberyo@gmail.com
