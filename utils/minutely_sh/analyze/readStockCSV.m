function [date, minites, hajimene, takane, yasune, owarine] = readStockCSV(filename)
data =csvread('../../minutely_sh/7203/20151109'); # 一番最後の要素は必ず全部埋まっていること．
date=data(:, 1);
minites=data(:, 2);
hajimene=data(:, 3);
takane=data(:, 4);
yasune=data(:, 5);
owarine=data(:, 6);

% break time compensation
for i_inv = 1:size(owarine)-1
    i = size(owarine, 1) - i_inv;
    if owarine(i) == 0
        owarine(i) = owarine(i+1);
    endif 
end
end
