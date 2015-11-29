#include <iostream>
#include <cstdio>
#include "Agent.hpp"

Agent::Agent(double max_consignment_guarantee_money)
{
    clear();
    m_max_consignment_guarantee_money = max_consignment_guarantee_money;
}

void Agent::clear(void)
{
    m_profit = 0;
    m_stocks.clear();
    m_buying_timing.clear();
    m_selling_timing.clear();
}
void Agent::addHoldingStock(HoldingStock* hs)
{
    m_stocks[hs->m_code] = hs;
}

HoldingStock* Agent::getHoldingStock(int code)
{
    return m_stocks[code];
}

int Agent::buy(int code, double time, double num, double price, double sl, double tp)
{
    double sum_consignment = 0;
    for (std::map<int,HoldingStock*>::iterator it=m_stocks.begin(); it!=m_stocks.end(); ++it) {
        if (it->first == code)
            sum_consignment += abs(m_stocks[code]->expectedConsignment(num, price));
        else 
            sum_consignment += abs(m_stocks[it->first]->getNeededConsignmentGuaranteeMoney());
    }

    if (sum_consignment > m_max_consignment_guarantee_money) 
        return 1;
//    std::cout << code << " " << time << " " << num << " " << price << " " << m_stocks[code]->expectedConsignment(+num, price) << " " << m_max_consignment_guarantee_money << std::endl;

    m_profit += m_stocks[code]->buy(num, price, sl, tp);
    m_buying_timing.push_back(time);

    return 0; 
}

int Agent::sell(int code, double time, double num, double price, double sl, double tp)
{
    double sum_consignment = 0;
    for (std::map<int,HoldingStock*>::iterator it=m_stocks.begin(); it!=m_stocks.end(); ++it) {
        if (it->first == code)
            sum_consignment += abs(m_stocks[code]->expectedConsignment(-num, price));
        else 
            sum_consignment += abs(m_stocks[it->first]->getNeededConsignmentGuaranteeMoney());
    }

    if (sum_consignment > m_max_consignment_guarantee_money) 
        return 1;

//    std::cout << code << " " << time << " " << -num << " " << price << " " << m_stocks[code]->expectedConsignment(-num, price) << " " << m_max_consignment_guarantee_money << std::endl;
    m_profit += m_stocks[code]->sell(num, price, sl, tp);
    m_selling_timing.push_back(time);

    return 0; 
}


double Agent::getProfit(void)
{
    return m_profit;
}

void Agent::ForcedSettlement(int code, double time, double price)
{
    double profit = m_stocks[code]->ForcedSettlement(price);
    m_profit += profit;
    if (profit) 
        m_selling_timing.push_back(time);
}

void Agent::tradeBySLTP(int code, double time, double price)
{
    double profit = m_stocks[code]->tradeBySLTP(price);
    m_profit += profit;
    if (profit) 
        m_selling_timing.push_back(time);
}

int Agent::getTradeNum(void)
{
    return m_buying_timing.size() + m_selling_timing.size();
}
