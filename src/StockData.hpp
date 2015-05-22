#ifndef __STOCK_DATA_HPP_INCLUDED__
#define __STOCK_DATA_HPP_INCLUDED__

#include <map>
#include <string>
#include "StockDayStream.hpp"

class StockData {
private:
    std::map<std::string, StockDayStream*> stock_;
public:
    void addStockDay(std::string code, std::string market, std::string name, std::string category, StockDay* sd);
    StockDayStream* getStockDayStream(std::string code);
    void sortAll(void);
    void save(void);
    ~StockData(void);
};
#endif

