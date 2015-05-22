#include "DataWarehouse_loader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>
#include "date.hpp"
#include "StockData.hpp"

void DataWarehouse_loader::load(StockData* data) 
{
    system("ls -1 ../data/dd/*.CSV > tmp_filename");

    std::ifstream filenames("tmp_filename");
    std::string filename;
    while (filenames && getline(filenames, filename)) {
        std::cerr << "#" << filename << std::endl;
        std::ifstream codefile(filename);
        std::string b;
        while (codefile && getline(codefile, b)) {
            std::istringstream is(b);
            std::string tmp[16]; 
            std::string str;
            int i = 0;
            while (getline(is, str, ',')) {
                tmp[i] = str;
                i++;
            }
            if (i != 8) { 
                std::cerr << "# load warning " << b << " " << filename << std::endl;
                continue; // データが予想外
            }

            int y = atoi(tmp[0].substr(0, 4).c_str());
            int m = atoi(tmp[0].substr(5, 2).c_str());
            int d = atoi(tmp[0].substr(8, 2).c_str());
//            std::cout << y << " " << m << " " << d << " " << ymdtoi(atoi(tmp[0].substr(0, 4).c_str()), atoi(tmp[0].substr(5, 2).c_str()), atoi(tmp[0].substr(8, 2).c_str())) << std::endl; // day
            data->addStockDay(filename.substr(11, 4), "", "", "", // code, market, name, category
                    new StockDay(
                        ymdtoi(y, m, d), // day
                        atoi(tmp[3].c_str()), atoi(tmp[2].c_str()), atoi(tmp[1].c_str()), atoi(tmp[4].c_str()), atoi(tmp[5].c_str()), -1)); // low, high, opening, closing, value, volume
        }
    }

    data->sortAll();
    system("rm tmp_filename");
}
