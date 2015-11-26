#pragma once
#include <vector>

#define LEASQR_ITER 5  // 精度と計算量の兼ね合い。以前は20だったが…？
#define LEASQR_LINE_RESOLUTION 100  // 精度と計算量の兼ね合い。以前は20だったが…？
#define LEASQR_LINE_SEARCH_DIR_NUM 4

enum {
    LEASQR_LINE_UPPER,
    LEASQR_LINE_LOWER,
    MAX_LEASQR_NUM_
};

int leasqr_line(const std::vector<double>& mx_org, const std::vector<double>& my_in_org, int start_index, int len, int dir, double line_coeff, double* a, double* b);
