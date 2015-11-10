filename = '../7203/20151110';
[date, minites, hajimene, takane, yasune, owarine] = readStockCSV(filename);

money_first = 10000000; % [yen]
money = money_first; % [yen]
stock_num = 0;
volume = 0;
trade_num = 0;

trade_threshold = 0.05;
for oi = 1:size(owarine)
	o = owarine(oi);
	if (rand() < trade_threshold && money > o * 100) 
		money -= o * 100;
		stock_num += 100;
		volume += o * 100;
		trade_num++;
	end
	if (rand() > 1 - trade_threshold && stock_num > 0) 
		money += o * 100;
		stock_num -= 100;
		volume += o * 100;
		trade_num++;
	end

end

% Commition
commission = getCommission(volume);
money -= commission;

% Calc results
money_end = money + stock_num * owarine(end);
profit = money_end - money_first;
gap = owarine(end) - owarine(1);

% Print results
printf('%f %d %d %f %f\n', profit, trade_num, commission, gap, gap/owarine(1));
