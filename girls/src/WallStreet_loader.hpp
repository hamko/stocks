#pragma once 

#include <string>
#include "Loader.hpp"
class WallStreet_loader : public Loader {
private:
    std::string filename;
public: 
    virtual void load(std::string database);
};
