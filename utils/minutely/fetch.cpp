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

int from_y = 2015, from_m = 8, from_d = 16;
int to_y = 2015, to_m = 8, to_d = 25;

int main(int argc, char** argv)
{
    omp_set_num_threads(10);

    stringstream ss;
//#pragma omp parallel for
    for (int y = 2015; y > 2006; y--) {
//    int y = atoi(argv[1]);
        for (int m = 12; m >= 1; m--) { // 12, 1
            for (int d = 31; d >= 1; d--) {
                if (from_y > y || (from_y == y && from_m > m) || (from_y == y && from_m == m && from_d > d))
                    continue;
                if (to_y < y || (to_y == y && to_m < m) || (to_y == y && to_m == m && to_d < d))
                    continue;
                if (!checkdate(m, d, y))
                    continue;

                for (int s = 4005; s <= 4005; s++) {
                    cout << y << zero(m) << m << zero(d) << d << endl;
                    ss.str(std::string());
                    //                ss << "wget -q http://k-db.com/stocks/" << y << "-" << zero(m) << m << "-" << zero(d) << d << "?download=csv -O" << y << zero(m) << m << zero(d) << d;
                    ss << "wget -q 'http://k-db.com/stocks/" << s << "-T/minutely?date="<< y << "-" << zero(m) << m << "-" << zero(d) << d << "&download=csv' -O" << y << zero(m) << m << zero(d) << d;
                    //http://k-db.com/stocks/4005-T/minutely?date=2015-08-25&download=csv
                    cout << ss.str() << endl;
                                        system(ss.str().c_str());
                                        sleep(1);
                }
            }
        }
    }
}
