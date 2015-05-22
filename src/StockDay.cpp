#include "StockDay.hpp"

StockDay::StockDay(int day, int lowest, int highest, int opening, int closing, int value, int volume)
    : day_(day), lowest_(lowest), highest_(highest), opening_(opening), closing_(closing), value_(value), volume_(volume)
{
}

StockDay::StockDay()
{
}
