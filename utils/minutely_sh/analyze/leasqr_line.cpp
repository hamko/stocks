#include "leasqr_line.hpp"
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

void leasqr_line(const vector<double>& mx_org, const vector<double>& my_in_org, int start_index, int len, int dir, double line_coeff, double* a, double* b)
{
    vector<double> mx;
    vector<double> my_in;
    for (int i = start_index; i < start_index + len; i++) {
        mx.push_back(mx_org[i]);
        my_in.push_back(my_in_org[i]);
    }

    double mean_mx = 0;
    double absmax_mx = -1e8;
    for (int i = 0; i < len; i++) {
        mean_mx += mx[i];
        absmax_mx = max(absmax_mx, abs(mx[i]));
    }
    mean_mx /= len;
    for (int i = 0; i < len; i++) {
        mx[i] -= mean_mx;
        mx[i] /= absmax_mx;
    }

    double mean_my_in = 0;
    double absmax_my_in = -1e8;
    for (int i = 0; i < len; i++) {
        mean_my_in += my_in[i];
        absmax_my_in = max(absmax_my_in, abs(my_in[i]));
    }
    mean_my_in /= len;
    for (int i = 0; i < len; i++) {
        my_in[i] -= mean_my_in;
        my_in[i] /= absmax_my_in;
    }

    double max_my_in = -1e8;
    double min_my_in = +1e8;
    for (int i = 0; i < len; i++) {
        max_my_in = max(max_my_in, my_in[i]);
        min_my_in = min(min_my_in, my_in[i]);
    }
    if (dir == LEASQR_LINE_LOWER) {
        *a = 0;
        *b = min_my_in;
    } else { 
        *a = 0;
        *b = max_my_in;
    }

    double min_vysq = 1e8;
    double search_dir_a[LEASQR_LINE_SEARCH_DIR_NUM]={1,-1,0,0};
    double search_dir_b[LEASQR_LINE_SEARCH_DIR_NUM]={0,0,1,-1};
    for (int i = 0; i < LEASQR_LINE_SEARCH_DIR_NUM*LEASQR_LINE_RESOLUTION; i++) {
        double cand_a = *a + search_dir_a[i % LEASQR_LINE_SEARCH_DIR_NUM] / (LEASQR_LINE_SEARCH_DIR_NUM*LEASQR_LINE_RESOLUTION);
        double cand_b = *b + search_dir_b[i % LEASQR_LINE_SEARCH_DIR_NUM] / (LEASQR_LINE_SEARCH_DIR_NUM*LEASQR_LINE_RESOLUTION);
        double vysq = 0;
        for (int j = 0; j < len; j++) {
            double vy = cand_a * mx[j] + cand_b - my_in[j];
            if (dir == LEASQR_LINE_LOWER && vy > 0)  
                vy *= line_coeff;
            if (dir == LEASQR_LINE_UPPER && vy < 0)  
                vy *= line_coeff;
            vysq += vy * vy;
        }
     
//        cout << i << " " << cand_a << " " << cand_b << " " << vysq << endl;
        if (min_vysq > vysq) {
            min_vysq = vysq;
            *a = cand_a;
            *b = cand_b;
        }
    }

    // yo = a * xo + b
    // (y - ym) / ys = a * (x - xm) / xs + b
    // y=ys*a(x-xm)/xs+ys*b+ym
    // y=ys*a*x/xs-ys*a*xm/xs+ys*b+ym
    double a_prev = *a, b_prev = *b;
    *a = +absmax_my_in * a_prev / absmax_mx;
    *b = -absmax_my_in * a_prev * mean_mx / absmax_mx + absmax_my_in * b_prev + mean_my_in;
}
