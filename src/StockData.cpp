#include "StockData.hpp"
#include <iostream>

void StockData::addStockDay(std::string code, std::string market, std::string name, std::string category, StockDay* sd)
{
    if (!stock_.count(code)) {
        stock_[code] = new StockDayStream(code, market, name, category); 
    }
    stock_[code]->addStockDay(sd);
}

StockDayStream* StockData::getStockDayStream(std::string code)
{
    if (!stock_.count(code)) {
        std::cerr << "No such code" << " " << __FILE__ << " " << __LINE__ << std::endl;
    }

    return stock_[code];
}

void StockData::sortAll(void)
{
    for (std::map<std::string, StockDayStream*>::iterator it = stock_.begin(); it != stock_.end(); it++) {
//        std::cout << it->second->getCode() << std::endl;
        it->second->sort();
    }
}

StockData::~StockData(void)
{
    for (std::map<std::string, StockDayStream*>::iterator it = stock_.begin(); it != stock_.end(); it++) {
        delete it->second;
    }
}

void StockData::save(void)
{
    // TODO
}
