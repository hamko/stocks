#ifndef __LOADER_HPP_INCLUDED__
#define __LOADER_HPP_INCLUDED__
#include "StockData.hpp"
#include <string>
class Loader {
protected:
    std::string filename;
public:
    virtual void load(StockData* data) = 0;
};
#endif
