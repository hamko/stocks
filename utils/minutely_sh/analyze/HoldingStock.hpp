#pragma once

class HoldingStock {
public:
    int m_code;
    HoldingStock(int code);
    double m_num;
    double m_sl;
    double m_tp;
    double m_price; // 持っていないときは0にしておく。
    double getNeededConsignmentGuaranteeMoney(void);
    void setSLTP(double sl, double tp);
    double buy(double num, double price, double sl, double tp);
    double sell(double num, double price, double sl, double tp);
    double trade(double num, double price, double sl, double tp);
    void clear(void);
};
