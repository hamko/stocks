#pragma once
#include "decision.hpp"

class decisionCross : public decision {
    int short_term_tau;
    int medium_term_tau;
    int analysis_period;
public:
    decisionCross(StockDayStream* stock, int s, int m, int a);
    virtual void update(stock_itr day);
};
