#include "WallStreet_loader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>
#include "date.hpp"

void WallStreet_loader::load(std::string database) 
{
    system("ls -1 ../data/*20*.txt > tmp_filename");

    std::ifstream filenames("tmp_filename");
    std::string filename;
    while (filenames && getline(filenames, filename)) {
//        std::cout << filename << std::endl;
        std::ifstream dayfile(filename);
        std::string b;
        getline(dayfile, b); // 初めに日付
        std::cerr << b << std::endl;
        while (dayfile && getline(dayfile, b)) {
            std::istringstream is(b);
            std::string tmp[16]; 
            std::string str;
            int i = 0;
            while (getline(is, str, '\t')) {
                tmp[i] = str;
                i++;
            }
            if (i < 3) { 
                std::cerr << "# load warning " << b << " " << filename << std::endl;
                continue; // データが予想外
            }

//            std::cout << tmp[0] << " " << ymdtoi(atoi(filename.substr(8, 4).c_str()), atoi(filename.substr(12, 2).c_str()), atoi(filename(14, 2).c_str())) << std::endl;
//            std::cout << ((std::string)"20"+filename.substr(9, 2)).c_str() << std::endl;
//           std::cout << ymdtoi(atoi(((std::string)"20"+filename.substr(9, 2)).c_str()), atoi(filename.substr(11, 2).c_str()), atoi(filename.substr(13, 2).c_str())) << std::endl;

//            data->addStockDay(tmp[0], "", tmp[1], "", new StockDay("not yet"/*ymdtoi(atoi(filename.substr(9, 4).c_str()), atoi(filename.substr(13, 2).c_str()), atoi(filename.substr(15, 2).c_str()))*/, atoi(tmp[3].c_str()), atoi(tmp[4].c_str()), atoi(tmp[2].c_str()), atoi(tmp[5].c_str()), -1, -1));
        }
    }

    system("rm tmp_filename");
}
