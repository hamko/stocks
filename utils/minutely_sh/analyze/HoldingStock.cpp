#include <iostream>
#include <algorithm>
#include <limits>
#include <cmath>
#include "HoldingStock.hpp"

HoldingStock::HoldingStock(int code)
    : m_code(code)
{
    this->clear();
}

double HoldingStock::getNeededConsignmentGuaranteeMoney(void)
{
    return m_price * m_num;
}

void HoldingStock::setSLTP(double sl, double tp)
{
    if (!isnan(sl)) m_sl = sl;
    if (!isnan(tp)) m_tp = tp;
}

double HoldingStock::expectedConsignment(double num, double price)
{
    if (m_num * num < 0) {
        if (abs(m_num) < abs(num)) { // Holding -200, buy 300
            return (m_num+num) * price;
        } else if (m_num + num == 0) { // Holding -200, buy 200
            return 0;
        } else { // Holding -200, buy 100
            return (m_num+num) * m_price;
        }
    } else { // Holding 0, buy 300 or Holding 100, buy 300
        return m_price * m_num + price * num;
    }
}

double HoldingStock::trade(double num, double price, double sl, double tp)
{
    if (!isnan(sl)) m_sl = sl; if (!isnan(tp)) m_tp = tp;

    double profit = 0;
    if (m_num * num < 0) {
        if (abs(m_num) < abs(num)) { // Holding -200, buy 300
            profit += m_num * (price - m_price);
            m_price = price;
            m_num += num;
        } else if (m_num + num == 0) { // Holding -200, buy 200
            profit += m_num * (price - m_price);
            m_price = 0;
            m_sl = m_tp = NAN;
            m_num = 0;
        } else { // Holding -200, buy 100
            profit += -num * (price - m_price);
            m_num += num;
        }
    } else { // Holding 0, buy 300 or Holding 100, buy 300
        m_price = (m_price * m_num + price * num) / (m_num + num);
        m_num += num;
    }

    return profit;
   
}

double HoldingStock::buy(double num, double price, double sl, double tp)
{
    return trade(num, price, sl, tp);
}

double HoldingStock::sell(double num, double price, double sl, double tp)
{
    return trade(-num, price, sl, tp);
}

void HoldingStock::clear(void)
{
    m_num = 0; m_price = 0; m_sl = NAN; m_tp = NAN;
}

double HoldingStock::ForcedSettlement(double price)
{
    return trade(-m_num, price, NAN, NAN);
}

double HoldingStock::tradeBySLTP(double price)
{
    double profit = 0;
    if (!std::isnan(m_sl) && m_num > 0 && m_sl >= price)
        profit += ForcedSettlement(price);
    if (!std::isnan(m_sl) && m_num > 0 && m_tp <= price)
        profit += ForcedSettlement(price);
    if (!std::isnan(m_sl) && m_num < 0 && m_sl <= price)
        profit += ForcedSettlement(price);
    if (!std::isnan(m_sl) && m_num < 0 && m_tp >= price)
        profit += ForcedSettlement(price);
    return profit;
}

