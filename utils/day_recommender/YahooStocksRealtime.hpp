#pragma once
#include <string>
//#include <tidy/tidy.h>
//#include <tidy/buffio.h>
#include <tidy.h>
#include <buffio.h>
#include <curl/curl.h>

class YahooStocksRealtime {
private:
    double comma_atof(unsigned char* bp);
public:
    double m_price;
    YahooStocksRealtime(int code, std::string market);
    void extractRealtimeStockInfo(TidyDoc doc, TidyNode tnod, int indent);
};
