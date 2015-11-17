$0!~"#"&&$0~" "&&$0!~"CONVERGENCE" {print; n+=$2; nb+=$3;nl++;}

END{printf("profit:%d,  tradenum:%d, days:%d, profit/trade:%.2f\n",n, nb, nl, (float)n/nb);}
