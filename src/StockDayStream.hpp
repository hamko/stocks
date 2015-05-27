#ifndef __STOCK_DAY_STREAM_HPP_INCLUDED__
#define __STOCK_DAY_STREAM_HPP_INCLUDED__
#include <vector>
#include <string>
#include "StockDay.hpp"

typedef std::vector<StockDay*>::iterator stock_itr;

class StockDayStream {
private:
    std::string code_;
    std::string market_;
    std::string name_;
    std::string category_;
public:
    std::vector<StockDay*> stock_day_;

    void addStockDay(StockDay* sd);
    void sort(void);
    std::string getCode(void){return code_;}
    void setCode(std::string code){code_ = code;}
    std::string getMarket(void){return market_;}
    void setMarket(std::string market){market_ = market;}
    std::string getName(void){return name_;}
    void setName(std::string name){name_ = name;}
    size_t size(void);

    stock_itr begin(void);
    stock_itr end(void);

    StockDayStream(std::string code, std::string market, std::string name, std::string category);
    StockDayStream(void);
    ~StockDayStream(void);
};
#endif
