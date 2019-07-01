#include "k_db_com_loader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include "date.hpp"

void k_db_com_loader::load(std::string database)
{
    system("ls -1 ../data/20* > tmp_filename");

    std::ifstream filenames("tmp_filename");
    std::string filename;
    while (filenames && getline(filenames, filename)) {
//        std::cout << filename << std::endl;
        std::ifstream dayfile(filename);
        std::string b;
        while (dayfile && getline(dayfile, b)) {
            std::istringstream is(b);
            std::string tmp[16]; 
            std::string str;
            int i = 0;
            while (getline(is, str, ',')) {
                tmp[i] = str;
                i++;
            }
            if (i < 5) continue; // 1行目エラー
            if (tmp[0] == "コード") continue; // 1行目エラー

//            std::cout << tmp[0] << " " << ymdtoi(atoi(filename.substr(8, 4).c_str()), atoi(filename.substr(12, 2).c_str()), atoi(filename(14, 2).c_str())) << std::endl;
//            std::cout << ymdtoi(atoi(filename.substr(8, 4).c_str()), atoi(filename.substr(12, 2).c_str()), atoi(filename.substr(14, 2).c_str())) << std::endl;

//            data->addStockDay(tmp[0], tmp[1], tmp[2], tmp[3], new StockDay("not yet"/*ymdtoi(atoi(filename.substr(8, 4).c_str()), atoi(filename.substr(12, 2).c_str()), atoi(filename.substr(14, 2).c_str()))*/, atoi(tmp[4].c_str()), atoi(tmp[5].c_str()), atoi(tmp[6].c_str()), atoi(tmp[7].c_str()), atoi(tmp[8].c_str()), atoi(tmp[9].c_str())));
        }
    }

    system("rm tmp_filename");
}
