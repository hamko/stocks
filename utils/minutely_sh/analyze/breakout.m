addpath("~/git/sample/octave");

% filename = '../7203/20151110';
filename = '../6146/20151110';
[date, minites, hajimene, takane, yasune, owarine] = readStockCSV(filename);

global money_first; global money; global stock_num; global volume; global trade_num;
money_first = 10000000; % [yen]
money = money_first; % [yen]
stock_num = 0;
volume = 0;
trade_num = 0;

function ret = buy(price, num)
    global money_first; global money; global stock_num; global volume; global trade_num;
	if (money < price * num) % Credit limit
        ret = 1;
        return;
    end
    money -= num * price;
    stock_num += num;
    volume += price * num;
    trade_num++;
end

function ret = sell(price, num)
    global money_first; global money; global stock_num; global volume; global trade_num;
%	if (money < price * num) % Credit limit
%        ret = 1;
%        return;
%    end
    money += num * price;
    stock_num -= num;
    volume += price * num;
    trade_num++;
end

threshold = 0;
for oi = 1:size(owarine)
	o = owarine(oi);
    if (oi == 3) 
        if (owarine(3) - owarine(1) > threshold)
            buy(o, 100); 
        elseif (owarine(3) - owarine(1) < -threshold)
            sell(o, 100); 
        end
    end
    len = 10; coeff = 10;
%    if (oi == 95)
    if (oi >= len+1)
        plot([1:oi]', owarine(1:oi), 'k'); hold on; 
        [a, b] = leasqr_line([oi-len:oi]', owarine(oi-len:oi), 0, coeff);
        plot([oi-len:oi]', a*[oi-len:oi]+b, 'b'); hold on; 
        [a, b] = leasqr_line([oi-len:oi]', owarine(oi-len:oi), 1, coeff);
        xlim([0 380]); 
        ylim([min(owarine) max(owarine)]); 
        plot([oi-len:oi]', a*[oi-len:oi]+b, 'r'); hold off; 
        pause(0.001);
    end
end
pause(10)

% Obliged Trade
if (stock_num != 0)
    money += stock_num * owarine(end);
    stock_num = 0;
    trade_num++;
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
