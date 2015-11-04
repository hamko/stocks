#!/bin/bash

cat `date +"%Y%m%d" --date "1 day ago"` | mail -s `date +"%Y%m%d" --date "1 day ago"` wakataberyo@gmail.com
