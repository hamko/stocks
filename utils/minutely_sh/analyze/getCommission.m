function ret=getCommission(price)
if (price < 100000)
	ret = 300;
elseif (price < 300000)
	ret = 500;
elseif (price < 500000)
	ret = 1000;
elseif (price < 100000000)
	ret = 1000 * ceil(price/1000000);
else
	ret = 100000;
end

end
