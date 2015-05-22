#pragma once
#include "StockDayStream.hpp"

class decision {
protected:
    double buy_;
    double tau_; // 時定数 [day]
    double credibility_; // 信頼度
    StockDayStream* stock_;
public: 
    decision(StockDayStream* stock);
    double getBuy(void){return buy_;}
    void setBuy(double buy){buy_ = buy;}
    double getTau(void){return tau_;}
    void setTau(double tau){tau_ = tau;}
    double getCredibility(void){return credibility_;}
    void setCredibility(double credibility){credibility_ = credibility;}
    virtual void update(stock_itr day) = 0;
};
