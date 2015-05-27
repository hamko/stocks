#include "StockDayStream.hpp"
#include <algorithm>

void StockDayStream::addStockDay(StockDay* sd)
{
    stock_day_.push_back(sd);
}

void StockDayStream::sort(void)
{
    std::sort(stock_day_.begin(), stock_day_.end(), [](StockDay* a, StockDay* b) {return a->getDay() < b->getDay();});
}

StockDayStream::~StockDayStream(void)
{
    for (std::vector<StockDay*>::iterator it = stock_day_.begin(); it != stock_day_.end(); it++) {
        delete (*it);
    }
}
    
StockDayStream::StockDayStream(void)
{
}



StockDayStream::StockDayStream(std::string code, std::string market, std::string name, std::string category)
    : code_(code), market_(market), name_(name), category_(category)
{
}

stock_itr StockDayStream::begin(void)
{
    return stock_day_.begin();
}

stock_itr StockDayStream::end(void)
{
    return stock_day_.end();
}

size_t StockDayStream::size(void)
{
    return stock_day_.size();
}
