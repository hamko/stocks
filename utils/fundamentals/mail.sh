#!/bin/bash

cp `date +"%Y%m%d" --date "1 day ago"` a
octave analize.m

echo "おはようございます。スイング担当の待鳥すいです。" >> tmp
echo `date +"%Y年%d月%m日のファンデメンタル情報をお伝えします。" --date "1 day ago"` >> tmp

echo >> tmp
echo "市場の過熱感の報告です。" >> tmp
awk '$0!~"#"&&$0~" "{printf("市場全体は、平均%.2fσ、中央値%.2fσの状態にあります。\n", $1, $2);}' M_Market.dat >> tmp
awk '$0!~"#"&&$0~" "{printf("レーティングの高い会社に限定すると、平均%.2fσ、中央値%.2fσの状態にあります。\n", $1, $2);}' M_GoodMarket.dat >> tmp

echo >> tmp
echo "割安株の報告です。" >> tmp
awk '$0!~"#"&&$0~" "{printf("銘柄番号%dが%.1fσで割安です。妥当株価の%.1f%です。（%.1f by %d）\nhttp://kabuyoho.ifis.co.jp/index.php?action=tp1&sa=report_per&bcode=%d\n", $1, $2, $6*100, $5, $4, $1);}' M_Economical.dat >> tmp

echo >> tmp
echo "割高株の報告です。" >> tmp
awk '$0!~"#"&&$0~" "{printf("銘柄番号%dが%.1fσで割高です。妥当株価の%.1f%です。（%.1f by %d）\nhttp://kabuyoho.ifis.co.jp/index.php?action=tp1&sa=report_per&bcode=%d\n", $1, $2, $6*100, $5, $4, $1);}' M_Expensive.dat >> tmp

echo >> tmp
echo "以上です。今日も一日頑張ってください。" >> tmp

cat tmp | mail -s `date +"%Y/%m/%d" --date "1 day ago"` wakataberyo@gmail.com 
cp tmp `date +"%Y%m%d" --date "1 day ago"`.report 
cp M.dat `date +"%Y%m%d" --date "1 day ago"`_M.dat

rm tmp
