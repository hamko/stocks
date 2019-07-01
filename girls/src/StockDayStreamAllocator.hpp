#pragma once 

#include <string>
#include "StockDayStream.hpp"

class StockDayStreamAllocator {
private:
    std::string database;
public:
    StockDayStream* newStockDayStream(std::string code, std::string day_begin, std::string day_end);
    StockDayStream* newStockDayMinStream(std::string code, std::string day);
    StockDayStreamAllocator(std::string _database){database = _database;}
};
