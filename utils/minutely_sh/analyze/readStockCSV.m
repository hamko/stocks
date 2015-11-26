function [date, minites, hajimene, takane, yasune, owarine] = readStockCSV(filename)
data =csvread(filename); # 一番最後の要素は必ず全部埋まっていること．
date=data(:, 1);
minites=data(:, 2);
hajimene=data(:, 3);
takane=data(:, 4);
yasune=data(:, 5);
owarine=data(:, 6);
owarine = flipud(owarine);
minites = flipud(minites);
hajimene = flipud(hajimene);
takane = flipud(takane);
yasune = flipud(yasune);

% break time compensation TODO use ending price!
for i_inv = 2:size(owarine)
    if owarine(i_inv) == 0
        owarine(i_inv) = owarine(i_inv-1);
    endif 
end
for i_inv = 2:size(owarine)-1
    i = size(owarine, 1) - i_inv;
    if owarine(i) == 0
        owarine(i) = owarine(i+1);
    endif 
end
end
