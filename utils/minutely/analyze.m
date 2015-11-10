global getResistance;
data =csvread('all'); # 一番最後の要素は必ず全部埋まっていること．
takane=data(:, 4);
yasune=data(:, 5);

for i_inv = 1:size(takane)-1
    i = size(takane, 1) - i_inv;
    if takane(i) == 0
        takane(i) = takane(i+1);
    endif 
end

plot(fliplr(takane')')

[a, b] = getResistance(takane(1:25), 'uwane'); 
#[a, b] = getResistance(yasune(1:25), 'shitane');
