#pragma once
#include <string>
#include <tidy/tidy.h>
#include <tidy/buffio.h>
#include <curl/curl.h>

#include "StockDay.hpp"

class YahooStocksRealtime {
private:
    StockDay data;
    double comma_atof(unsigned char* bp);
public:
    YahooStocksRealtime(int code, std::string market);
    void extractRealtimeStockInfo(TidyDoc doc, TidyNode tnod, int indent);
    double getPrices(void);
};
