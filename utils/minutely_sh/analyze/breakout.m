addpath("~/git/sample/octave");

filenames = {'../7203/20150818' '../7203/20150907' '../7203/20150930' '../7203/20151022'  '../7203/20150819' '../7203/20150908' '../7203/20151001' '../7203/20151023' '../7203/20150820' '../7203/20150909' '../7203/20151002' '../7203/20151026' '../7203/20150821' '../7203/20150910' '../7203/20151005' '../7203/20151027' '../7203/20150824' '../7203/20150911' '../7203/20151006' '../7203/20151028' '../7203/20150825' '../7203/20150914' '../7203/20151007' '../7203/20151029' '../7203/20150826' '../7203/20150915' '../7203/20151008' '../7203/20151030' '../7203/20150827' '../7203/20150916' '../7203/20151009' '../7203/20151102' '../7203/20150828' '../7203/20150917' '../7203/20151013' '../7203/20151104' '../7203/20150831' '../7203/20150918' '../7203/20151014' '../7203/20151105' '../7203/20150901' '../7203/20150924' '../7203/20151015' '../7203/20151106' '../7203/20150902' '../7203/20150925' '../7203/20151016' '../7203/20151109' '../7203/20150903' '../7203/20150928' '../7203/20151020' '../7203/20151110' '../7203/20150904' '../7203/20150929' '../7203/20151021'};
%filename = '../7203/20151102';
%filename = '../6146/20151106';
%filename = '../6146/20151109';
%filename = '../6146/20151110';

for filename_cell = filenames
filename = filename_cell{1};
[date, minites, hajimene, takane, yasune, owarine] = readStockCSV(filename);

global money_first; global money; global stock_num; global volume; global trade_num;
money_first = 3000000; % [yen]
money = money_first; % [yen]
stock_num = 0;
volume = 0;
trade_num = 0;

function ret = buy(price, num, oi)
    global money_first; global money; global stock_num; global volume; global trade_num;
	if (num == 0 | money_first < abs(price * (stock_num + num))) % Credit limit TODO
        ret = 1;
        return;
    end
    money -= num * price;
    stock_num += num;
    volume += price * num;
    trade_num++;

    printf('#buy %d stocks @ %f (%d)\n', num, price, oi);
end

function ret = sell(price, num, oi)
    global money_first; global money; global stock_num; global volume; global trade_num;
	if (num == 0 | money_first < abs(price * (stock_num - num))) % Credit limit TODO
        ret = 1;
        return;
    end
    money += num * price;
    stock_num -= num;
    volume += price * num;
    trade_num++;

    printf('#sell %d stocks @ %f (%d)\n', num, price, oi);
end

function ret = is_valid(oi)
    if (oi >= 152 & oi <= 210)
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
    len = 30; 
    coeff = 10; % あんまりいじらないこと
    if (oi >= len+1)
        [a_lower, b_lower] = leasqr_line([oi-len:oi]', owarine(oi-len:oi), 0, coeff);
        lower_slope(oi) = a_lower;
        [a_upper, b_upper] = leasqr_line([oi-len:oi]', owarine(oi-len:oi), 1, coeff);
        upper_slope(oi) = a_upper;

        % slope based trade
        upper_thres = 0.5; % about 1.0? TODO
        lower_thres = -0.5; % about 1.0?

        if (is_valid(oi))
            % 上値の急上昇が落ち着いた時に売り
            if (upper_slope(oi-1) < upper_thres & upper_slope(oi) > upper_thres)
                sell(o, 100, oi); 
                sell_timing = [sell_timing, oi];
            end
            % 下値が急降下が落ち着いて，傾きが戻ってきた時に売り
            if (lower_slope(oi-1) > lower_thres & lower_slope(oi) < lower_thres)
                buy(o, 100, oi); 
                buy_timing = [buy_timing, oi];
            end
        end

        % plot slopes
        %{
        plot([1:oi]', lower_slope, 'b'); hold on;
        plot([1:oi]', upper_slope, 'r'); hold off;

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
        %}
        % pause
%        pause(0.001);
    end
end
%pause

% Obliged Trade
if (stock_num > 0)
    sell(owarine(end), stock_num, size(owarine)(1)); 
elseif (stock_num < 0)
    buy(owarine(end), -stock_num, size(owarine)(1)); 
else
end

% Commition
commission = getCommission(volume);
money -= commission;

% Calc results
money_end = money + stock_num * owarine(end);
profit = money_end - money_first;
gap = owarine(end) - owarine(1);

% Print results
printf('%s %f %d %d %f %f\n', filename, profit, trade_num, commission, gap, gap/owarine(1));


end
