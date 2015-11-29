#include <cmath>
#include "getCommission.hpp"

double getCommission(double price)
{
    double ret = 0;
    if (price == 0)
        ret = 0;
    else if (price < 100000)
        ret = 300;
    else if (price < 300000)
        ret = 500;
    else if (price < 500000)
        ret = 1000;
    else if (price < 100000000)
        ret = 1000 * ceil(price/1000000);
    else
        ret = 100000;

    return ret;

}
