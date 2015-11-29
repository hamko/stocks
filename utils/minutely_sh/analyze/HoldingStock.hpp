#pragma once
#include <iostream>

class HoldingStock {
public:
    int m_code;
    HoldingStock(int code);
    std::ostream& operator()(std::ostream& ros) {
        ros << m_num << " " << m_price << " " << m_sl << " " << m_tp;
        return ros;
    }
    double m_num;
    double m_sl;
    double m_tp;
    double m_price; // 持っていないときは0にしておく。
    double getNeededConsignmentGuaranteeMoney(void);
    void setSLTP(double sl, double tp);
    double buy(double num, double price, double sl, double tp);
    double sell(double num, double price, double sl, double tp);
    double trade(double num, double price, double sl, double tp);
    double expectedConsignment(double num, double price);
    void clear(void);

    double ForcedSettlement(double price);
    double tradeBySLTP(double price);
};
