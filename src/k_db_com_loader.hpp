#ifndef __K_DB_COM_LOADER_HPP_INCLUDED__
#define __K_DB_COM_LOADER_HPP_INCLUDED__
#include <string>
#include "Loader.hpp"
class k_db_com_loader : public Loader {
private:
    std::string filename;
public: 
    virtual void load(StockData* data);

};
#endif
