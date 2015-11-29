#include "leasqr_line.hpp"
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

int leasqr_line(const vector<double>& mx_org, const vector<double>& my_org, int start_index, int len, int dir, double line_coeff, double* a, double* b)
{
    if (mx_org.size() != my_org.size()) {
        cerr << __FILE__ << ":" << __LINE__ << ": " << "Size must be equal" << endl;
        return 1;
    }
    if (len < 2) {
        cerr << __FILE__ << ":" << __LINE__ << ": " << "Length must be > 1)" << endl;
        return 1;
    }
    if (my_org.size() < (size_t)start_index + len || mx_org.size() < (size_t)start_index + len) {
        cerr << __FILE__ << ":" << __LINE__ << ": " << "Size too small (must be > 1)" << endl;
        return 1;
    }
    if (line_coeff <= 0) {
        cerr << __FILE__ << ":" << __LINE__ << ": " << "line_coeff must be positive" << endl;
        return 1;
    }

    vector<double> mx;
    vector<double> my;
    for (int i = start_index; i < start_index + len; i++) {
        if (std::isnan(mx_org[i]) || std::isnan(my_org[i]))
            return 1;
        mx.push_back(mx_org[i]);
        my.push_back(my_org[i]);
    }

    double mean_mx = 0;
    double absmax_mx = -1e8;
    for (int i = 0; i < len; i++) 
        mean_mx += mx[i];
    mean_mx /= len;
    for (int i = 0; i < len; i++) 
        mx[i] -= mean_mx;
    for (int i = 0; i < len; i++) 
        absmax_mx = max(absmax_mx, abs(mx[i]));
    for (int i = 0; i < len; i++) 
        mx[i] /= absmax_mx;

    double mean_my = 0;
    double absmax_my = -1e8;
    for (int i = 0; i < len; i++) 
        mean_my += my[i];
    mean_my /= len;
    for (int i = 0; i < len; i++) 
        my[i] -= mean_my;
    for (int i = 0; i < len; i++) 
        absmax_my = max(absmax_my, abs(my[i]));
    for (int i = 0; i < len; i++) 
        my[i] /= absmax_my;

    // Initial Value
    double max_my = -1e8;
    double min_my = +1e8;
    for (int i = 0; i < len; i++) {
        max_my = max(max_my, my[i]);
        min_my = min(min_my, my[i]);
    }
    if (dir == LEASQR_LINE_LOWER) {
        *a = 0;
        *b = min_my;
    } else { 
        *a = 0;
        *b = max_my;
    }

    double min_vysq = 1e8;
    double search_dir_a[LEASQR_LINE_SEARCH_DIR_NUM]={1,-1,0,0};
    double search_dir_b[LEASQR_LINE_SEARCH_DIR_NUM]={0,0,1,-1};
    for (int i = 0; i < LEASQR_LINE_SEARCH_DIR_NUM*LEASQR_LINE_RESOLUTION*LEASQR_ITER; i++) {
        double cand_a = *a + search_dir_a[i % LEASQR_LINE_SEARCH_DIR_NUM] / LEASQR_LINE_RESOLUTION;
        double cand_b = *b + search_dir_b[i % LEASQR_LINE_SEARCH_DIR_NUM] / LEASQR_LINE_RESOLUTION;
        double vysq = 0;
        for (int j = 0; j < len; j++) {
            double vy = cand_a * mx[j] + cand_b - my[j];
            if (dir == LEASQR_LINE_LOWER && vy > 0)  
                vy *= line_coeff;
            if (dir == LEASQR_LINE_UPPER && vy < 0)  
                vy *= line_coeff;
            vysq += vy * vy;
        }
     
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
    *a = +absmax_my * a_prev / absmax_mx;
    *b = -absmax_my * a_prev * mean_mx / absmax_mx + absmax_my * b_prev + mean_my;

    return 0;
}


