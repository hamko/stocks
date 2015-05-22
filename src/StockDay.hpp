#ifndef __STOCK_DAY_HPP_INCLUDED__
#define __STOCK_DAY_HPP_INCLUDED__
class StockDay {
private:
    int day_;
    int prices_;
    int lowest_;
    int highest_;
    int opening_;
    int closing_;
    int value_; // 売買代金
    int volume_; // 出来高
public:
    StockDay(int day, int lowest, int highest, int opening, int closing, int value, int volume);
    StockDay();

    int getDay(void){return day_;}
    void setDay(int day){day_ = day;}
    int getPrices(void){return prices_;}
    void setPrices(int prices){prices_ = prices;}
    int getLowest(void){return lowest_;}
    void setLowest(int lowest){lowest_ = lowest;}
    int getHighest(void){return highest_;}
    void setHighest(int highest){highest_ = highest;}
    int getOpening(void){return opening_;}
    void setOpening(int opening){opening_ = opening;}
    int getClosing(void){return closing_;}
    void setClosing(int closing){closing_ = closing;}
    int getValue(void){return value_;}
    void setValue(int value){value_ = value;}
    int getVolume(void){return volume_;}
    void setVolume(int volume){volume_ = volume;}
};
#endif
