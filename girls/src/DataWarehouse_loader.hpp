#pragma once 

#include <string>
#include "Loader.hpp"
class DataWarehouse_loader : public Loader {
private:
    std::string filename;
public: 
    virtual void load(std::string data);
};
