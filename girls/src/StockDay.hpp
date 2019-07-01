#ifndef __STOCK_DAY_HPP_INCLUDED__
#define __STOCK_DAY_HPP_INCLUDED__
#include <string>

class StockDay {
private:
    std::string day_; //2008-12-31 23:59:59 など 
    double prices_;
    double lowest_;
    double highest_;
    double opening_;
    double closing_;
    double value_; // 売買代金
    double volume_; // 出来高
public:
    StockDay(std::string day, double lowest, double highest, double opening, double closing, double value, double volume);
    StockDay();

    std::string getDay(void){return day_;}
    void setDay(std::string day){day_ = day;}
//    void setDayInteger(string day){day_ = day;}
    double getPrices(void){return prices_;}
    void setPrices(double prices){prices_ = prices;}
    double getLowest(void){return lowest_;}
    void setLowest(double lowest){lowest_ = lowest;}
    double getHighest(void){return highest_;}
    void setHighest(double highest){highest_ = highest;}
    double getOpening(void){return opening_;}
    void setOpening(double opening){opening_ = opening;}
    double getClosing(void){return closing_;}
    void setClosing(double closing){closing_ = closing;}
    double getValue(void){return value_;}
    void setValue(double value){value_ = value;}
    double getVolume(void){return volume_;}
    void setVolume(double volume){volume_ = volume;}
};
#endif
