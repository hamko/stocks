addpath("~/git/sample/octave");

%filename = '../7203/20151110';
%filename = '../6146/20151106';
filename = '../6146/20151109';
%filename = '../6146/20151110';
[date, minites, hajimene, takane, yasune, owarine] = readStockCSV(filename);

global money_first; global money; global stock_num; global volume; global trade_num;
money_first = 3000000; % [yen]
money = money_first; % [yen]
stock_num = 0;
volume = 0;
trade_num = 0;

function ret = buy(price, num)
    global money_first; global money; global stock_num; global volume; global trade_num;
	if (money_first < abs(price * (stock_num + num))) % Credit limit TODO
        ret = 1;
        return;
    end
    money -= num * price;
    stock_num += num;
    volume += price * num;
    trade_num++;

    printf('#buy %d stocks at %f\n', num, price);
end

function ret = sell(price, num)
    global money_first; global money; global stock_num; global volume; global trade_num;
	if (money_first < abs(price * (stock_num - num))) % Credit limit TODO
        ret = 1;
        return;
    end
    money += num * price;
    stock_num -= num;
    volume += price * num;
    trade_num++;

    printf('#sell %d stocks at %f\n', num, price);
end

function ret = is_valid(oi)
    if (oi >= 152 && oi <= 210)
        ret = 0;
    else
        ret = 1;
    end
end

upper_slope = [];
lower_slope = [];
buy_timing = [];
sell_timing = [];
threshold = 0;
for oi = 1:size(owarine)
	o = owarine(oi);
    len = 30; coeff = 10;
    if (oi >= len+1)
        [a_lower, b_lower] = leasqr_line([oi-len:oi]', owarine(oi-len:oi), 0, coeff);
        lower_slope(oi) = a_lower;
        [a_upper, b_upper] = leasqr_line([oi-len:oi]', owarine(oi-len:oi), 1, coeff);
        upper_slope(oi) = a_upper;

        % slope based trade
        upper_thres = 1;
        lower_thres = -1;

        if (is_valid(oi))
            % 上値の急上昇が落ち着いた時に売り
            if (upper_slope(oi-1) < upper_thres && upper_slope(oi) > upper_thres)
                sell(o, 100); 
                sell_timing = [sell_timing, oi];
            end
            % 下値が急降下が落ち着いて，傾きが戻ってきた時に売り
            if (lower_slope(oi-1) > lower_thres && lower_slope(oi) < lower_thres)
                buy(o, 100); 
                buy_timing = [buy_timing, oi];
            end
        end

        % plot slopes
%        plot([1:oi]', lower_slope, 'b'); hold on;
%        plot([1:oi]', upper_slope, 'r'); hold off;

        % plot stock price
        plot([1:oi]', owarine(1:oi), 'k'); hold on; 

        plot([oi-len:oi]', a_lower*[oi-len:oi]+b_lower, 'b'); hold on; 
        for bt = buy_timing
            scatter(bt, owarine(bt), 1);
        end
        for st = sell_timing
            scatter(st, owarine(st), 1);
        end

        xlim([0 380]); 
        ylim([min(owarine) max(owarine)]); 
        plot([oi-len:oi]', a_upper*[oi-len:oi]+b_upper, 'r'); hold off; 
        % pause
        pause(0.001);
    end
end
pause

% Obliged Trade
if (stock_num > 0)
    sell(owarine(end), stock_num); 
elseif (stock_num < 0)
    buy(owarine(end), -stock_num); 
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
