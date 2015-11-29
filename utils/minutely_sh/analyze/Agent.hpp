#pragma once
#include <vector>
#include <map>
#include "HoldingStock.hpp"

class Agent {
public:
    double m_max_consignment_guarantee_money;
    double m_profit;
    std::vector<double> m_buying_timing;
    std::vector<double> m_selling_timing;

    Agent(double max_consignment_guarantee_money);
    void clear(void);
    void addHoldingStock(HoldingStock* hs);
    std::map<int,HoldingStock*> m_stocks;
    HoldingStock* getHoldingStock(int code);
    int buy(int code, double time, double num, double price, double sl, double tp);
    int sell(int code, double time, double num, double price, double sl, double tp);
    double getProfit(void);
    void ForcedSettlement(int code, double time, double price);
    void tradeBySLTP(int code, double time, double price);
    int getTradeNum(void);
};
