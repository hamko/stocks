#include <ctime>
#include <cstdio>
#include <cmath>

// y[1900..2038], m[1..12], d[1..31] is sound
int ymdtoi(int y,int m,int d)
{
    struct tm t = {0,0,0,d,m-1,y-1900,-1,-1,-1};
    return (int)round(mktime(&t)/60/60/24);
}
