#include <iostream>
#include <sstream>
#include <cstdlib>
#include <omp.h>
#include "checkdate.hpp"
using namespace std;
#define zero(a) ((a)<10?"0":"")

#ifndef _OPENMP
#error "no openmp"
#endif

int from_y = 2015, from_m = 3, from_d = 19;

int main(int argc, char** argv)
{
    omp_set_num_threads(10);

    stringstream ss;
//#pragma omp parallel for
    for (int y = 2015; y > 2006; y--) {
//    int y = atoi(argv[1]);
        for (int m = 12; m >= 1; m--) {
            for (int d = 31; d >= 1; d--) {
                for (int s = 4005; s <= 4005; s++) {
                    if (from_y < y || (from_y == y && from_m < m) || (from_y == y && from_m == m && from_d < d))
                        continue;
                    if (!checkdate(m, d, y))
                        continue;
                    cout << y << zero(m) << m << zero(d) << d << endl;
                    ss.str(std::string());
                    //                ss << "wget -q http://k-db.com/stocks/" << y << "-" << zero(m) << m << "-" << zero(d) << d << "?download=csv -O" << y << zero(m) << m << zero(d) << d;
                    ss << "wget -q 'http://k-db.com/stocks/" << s << "-T/minutely?data="<< y << "-" << zero(m) << m << "-" << zero(d) << d << "?download=csv' -O" << y << zero(m) << m << zero(d) << d;
//http://k-db.com/stocks/4005-T/minutely?date=2015-08-25&download=csv
                    cout << ss.str() << endl;
                    system(ss.str().c_str());
                }
            }
        }
    }
}
