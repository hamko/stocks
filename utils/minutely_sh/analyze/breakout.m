visualize_flag = 0;
visualize_movie_flag = 0;

addpath("~/git/sample/octave");

filenames = {'../7203/20150818' '../7203/20150907' '../7203/20150930' '../7203/20151022'  '../7203/20150819' '../7203/20150908' '../7203/20151001' '../7203/20151023' '../7203/20150820' '../7203/20150909' '../7203/20151002' '../7203/20151026' '../7203/20150821' '../7203/20150910' '../7203/20151005' '../7203/20151027' '../7203/20150824' '../7203/20150911' '../7203/20151006' '../7203/20151028' '../7203/20150825' '../7203/20150914' '../7203/20151007' '../7203/20151029' '../7203/20150826' '../7203/20150915' '../7203/20151008' '../7203/20151030' '../7203/20150827' '../7203/20150916' '../7203/20151009' '../7203/20151102' '../7203/20150828' '../7203/20150917' '../7203/20151013' '../7203/20151104' '../7203/20150831' '../7203/20150918' '../7203/20151014' '../7203/20151105' '../7203/20150901' '../7203/20150924' '../7203/20151015' '../7203/20151106' '../7203/20150902' '../7203/20150925' '../7203/20151016' '../7203/20151109' '../7203/20150903' '../7203/20150928' '../7203/20151020' '../7203/20151110' '../7203/20150904' '../7203/20150929' '../7203/20151021'};
%filenames = {'../7203/20151102'};
%filenames = {'../6146/20151106'};
%filenames = {'../6146/20151109'};
%filenames = {'../7203/20150929'};

for filename_cell = filenames
filename = filename_cell{1};
[date, minites, hajimene, takane, yasune, owarine] = readStockCSV(filename);

global money_first; global money; global volume; global trade_num; global call_timing; global put_timing;
money_first = 3000000; % [yen]
money = money_first; % [yen]
volume = 0;
trade_num = 0;
minimum_unit = 300;
call_timing = [];
put_timing = [];

stocks = {};

function ret = amount()
    global money_first; global money; global volume; global trade_num, global stocks;
    ret = 0;
    for s = stocks
        ret += (s.price * s.num);
    end
end

function ret = call(price, num, oi, sl, tp)
    global money_first; global money; global volume; global trade_num, global stocks; global put_timing; global call_timing;

	if (num == 0 | money < abs(amount() + price * num)) % Credit limit TODO
        ret = 1;
        return;
    end
    volume += price * num;
    trade_num++;
    
    stock.price = price; stock.num = num; stock.minites = oi;
    stock.tp = tp; stock.sl = sl;
    stocks = [stocks stock];

    printf('#call %d stocks @ %f (%d, %f, %f)\n', num, price, oi, sl, tp);
    call_timing = [call_timing, oi];
    ret = 0;
end

function ret = put(price, num, oi, sl, tp)
    global money_first; global money; global volume; global trade_num; global stocks; global put_timing; global call_timing;
	if (num == 0 | money < abs(amount() - price * num)) % Credit limit TODO
        ret = 1;
        return;
    end
    volume += price * num;
    trade_num++;

    stock.price = price; stock.num = -num; stock.minites = oi;
    stock.tp = tp; stock.sl = sl;
    stocks = [stocks stock];

    printf('#put %d stocks @ %f (%d, %f, %f)\n', num, price, oi, sl, tp);
    put_timing = [put_timing, oi];
    ret = 0;
end

function resolveStock(index, price)
    global money_first; global money; global volume; global trade_num; global stocks;
    money += stocks(index).num * (price - stocks(index).price);
    volume += price * stocks(index).num;
    trade_num++;

    printf('#resolve %d stocks @ %f\n', stocks(index).num, price);
    stocks(index) = [];
end

function obligeTrading(price)
    global stocks;
    si = 0;
    while size(stocks, 2) > 0
        resolveStock(1, price);
    end
end

function takeProfit(price)
    global stocks;
    si = 0;
    for s = stocks
        si++;
        if (s.num > 0 & price > s.tp)
            resolveStock(si, price);
            printf('#takeProfit %d stocks @ %f > %f\n', s.num, price, s.tp);
            si = 0;
        elseif (s.num < 0 & price < s.tp)
            resolveStock(si, price);
            printf('#takeProfit %d stocks @ %f < %f\n', s.num, price, s.tp);
            si = 0;
        end
    end
end

function stopLoss(price)
    global stocks;
    si = 0;
    for s = stocks
        si++;
        if (s.num > 0 & price < s.sl)
            resolveStock(si, price);
            printf('#stopLoss %d stocks @ %f < %f\n', s.num, price, s.sl);
            si = 0;
        elseif(s.num < 0 & price > s.sl)
            resolveStock(si, price);
            printf('#stopLoss %d stocks @ %f > %f\n', s.num, price, s.sl);
            si = 0;
        end
    end
end


function ret = is_valid(oi)
    if (oi >= 152 & oi <= 210)
        ret = 0;
    else
        ret = 1;
    end
end

function ret = getMarketValue()
    global stocks;
    ret = 0;
    si = 0;
    for s = stocks
        si++;
        s = s{1};
        ret += s.price * s.num;
    end
end

upper_slope = [];
lower_slope = [];
threshold = 0;

if (visualize_flag)
    f1 = figure();
    f2 = figure();
end
for oi = 1:size(owarine)
	o = owarine(oi);
    stopLoss(o);
    takeProfit(o);
    len = 30; 
    coeff = 10; % あんまりいじらないこと
    if (oi >= len+1 & is_valid(oi) & is_valid(oi-len))
        [a_lower, b_lower] = leasqr_line([oi-len:oi]', owarine(oi-len:oi), 0, coeff);
        lower_slope(oi) = a_lower;
        [a_upper, b_upper] = leasqr_line([oi-len:oi]', owarine(oi-len:oi), 1, coeff);
        upper_slope(oi) = a_upper;

        % slope based trade
        upper_thres = 0.5; % about 1.0? TODO
        lower_thres = -0.5; % about 1.0?

        if (is_valid(oi))
            % 上値の急上昇が落ち着いた時に売り
            %{
            if (upper_slope(oi-1) < upper_thres & upper_slope(oi) > upper_thres)
                put(o, 100, oi); 
            end
            %}
            %{
            if (upper_slope(oi-1) < upper_thres & upper_slope(oi) > upper_thres)
                call(o, minimum_unit, oi, owarine(oi)-10, owarine(oi)+30); 
                call_timing = [call_timing, oi];
            end
            %}
            % 下値が急降下が落ち着いて，傾きが戻ってきた時に買い
            %{
            if (lower_slope(oi-1) > lower_thres & lower_slope(oi) < lower_thres)
                call(o, 100, oi); 
            end
            %}
            %{
            if (lower_slope(oi-1) > lower_thres & lower_slope(oi) < lower_thres)
                put(o, minimum_unit, oi, owarine(oi)+10, owarine(oi)-30); 
            end
            %}

            % Trend Trade
            if ((upper_slope(oi) + lower_slope(oi))/2 > 0.5)
                call(o, minimum_unit, oi, owarine(oi)-40, owarine(oi)+60); 
            end
            if ((upper_slope(oi) + lower_slope(oi))/2 < 0.5)
                put(o, minimum_unit, oi, owarine(oi)+40, owarine(oi)-60);
            end
            % BREAKOUT!
            if (upper_slope(oi-1)-lower_slope(oi-1) < 0.5 & upper_slope(oi) - lower_slope(oi) > 0.5)
%            if (upper_slope(oi) > 0.5 & lower_slope(oi) > 0.5 & upper_slope(oi-1)-lower_slope(oi-1) < 0.5 & upper_slope(oi) - lower_slope(oi) > 0.5)
                put(o, minimum_unit, oi, owarine(oi)+40, owarine(oi)-60);
            end
            if (upper_slope(oi-1)-lower_slope(oi-1) > -0.5 & upper_slope(oi) - lower_slope(oi) < -0.5)
%            if (upper_slope(oi) < -0.5 & lower_slope(oi) < -0.5 & upper_slope(oi-1)-lower_slope(oi-1) > -0.5 & upper_slope(oi) - lower_slope(oi) < -0.5)
                call(o, minimum_unit, oi, owarine(oi)-40, owarine(oi)+60); 
            end
        end

        % plot slopes
        if ((visualize_flag & visualize_movie_flag) | (visualize_flag & !visualize_movie_flag & oi == size(owarine, 1)))
            set(0,'CurrentFigure',f1);
            plot([1:oi]', lower_slope, 'b'); hold on; 
            plot([1:oi]', upper_slope, 'r'); 

            % plot stock price
            plot([1:oi]', owarine(1:oi), 'k');

            plot([oi-len:oi]', a_lower*[oi-len:oi]+b_lower, 'b');
            for bt = call_timing
                scatter(bt, owarine(bt), 'o');
            end
            for st = put_timing
                scatter(st, owarine(st), '*');
            end

            xlim([0 380]); 
            ylim([min(owarine) max(owarine)]); 
            plot([oi-len:oi]', a_upper*[oi-len:oi]+b_upper, 'r');
            hold off;

            set(0,'CurrentFigure',f2);
            hold on;
            xlim([0 380]); 
            plot([1:oi]', lower_slope, 'b');
            plot([1:oi]', upper_slope, 'r');
            plot([1:oi]', upper_slope-lower_slope, 'g');
            hold off;
            if (visualize_movie_flag)
                pause(0.001);
            end
        end
    end
end
if (visualize_flag)
    pause;
end

% Obliged Trade
obligeTrading(owarine(end));

% Commition
commission = getCommission(volume);
money -= commission;

% Calc results
money_end = money;
profit = money_end - money_first;
gap = owarine(end) - owarine(1);

% Print results
printf('%s %f %d %f %f\n', filename, profit+commission, profit, gap, gap/owarine(1));


end
