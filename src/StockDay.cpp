#include "StockDay.hpp"

StockDay::StockDay(std::string day, double lowest, double highest, double opening, double closing, double value, double volume)
    : day_(day), lowest_(lowest), highest_(highest), opening_(opening), closing_(closing), value_(value), volume_(volume)
{
}

StockDay::StockDay()
{
}
