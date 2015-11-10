global getResistance;
data =csvread('all'); # 一番最後の要素は必ず全部埋まっていること．
minites=data(:, 1);
date=data(:, 2);
hajimene=data(:, 3);
takane=data(:, 4);
yasune=data(:, 5);
owarine=data(:, 6);

for i_inv = 1:size(owarine)-1
    i = size(owarine, 1) - i_inv;
    if owarine(i) == 0
        owarine(i) = owarine(i+1);
    endif 
end

plot(fliplr(takane')')

[a, b] = getResistance(takane(1:25), 'uwane'); 
#[a, b] = getResistance(yasune(1:25), 'shitane');
