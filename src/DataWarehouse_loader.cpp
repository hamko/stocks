#include "DataWarehouse_loader.hpp"
#include <fstream>
#include <sstream>
#include <strstream>
#include <iostream>
#include <cstring>
#include <sqlite3.h>
#include "date.hpp"

void DataWarehouse_loader::load(std::string database) 
{
    system("ls -1 ../data/dd/*.CSV > tmp_filename");

    sqlite3 *db;
    char *zErrMsg = 0;
    int rc = sqlite3_open(database.c_str(), &db);
    sqlite3_exec(db, "BEGIN", NULL, NULL, NULL);

    std::ifstream filenames("tmp_filename");
    std::string filename;
    while (filenames && getline(filenames, filename)) {
        std::cerr << "#" << filename << std::endl;
        std::ifstream codefile(filename);
        std::string b;
        char insert_stock_templete[] = "INSERT INTO t_stock(code, name) values ('%s', '%s')";
        char insert_stock[1024];
        sprintf(insert_stock, insert_stock_templete, filename.substr(11, 4).c_str()/*code*/, ""/*name*/);
        rc = sqlite3_exec(db, insert_stock, 0, 0, &zErrMsg);

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
            std::strstream ss;
            ss << (y<10?"0":"") << y << "-" << (m<10?"0":"") << m << "-" << (d<10?"0":"") << d << std::ends;

            // データ追加SQL文
            char insert_day_templete[] = "INSERT INTO t_price(stock_code, time, prices, lowest, highest, opening, closing, value, volume)"
                "                values ('%s', '%s', %f, %f, %f, %f, %f, %f, %f)";
            char insert_day[1024];
            sprintf(insert_day, insert_day_templete, filename.substr(11, 4).c_str()/*code*/, ss.str()/*time day*/, 0/*price*/, atof(tmp[3].c_str())/*Lowest*/, atof(tmp[2].c_str())/*highest*/, atof(tmp[1].c_str())/*opening*/, atof(tmp[4].c_str()), atof(tmp[5].c_str()), -1);
            rc = sqlite3_exec(db, insert_day, 0, 0, &zErrMsg);
            //            std::cerr << zErrMsg << std::endl;
            //            std::cerr << (*itd)->getLowest() << std::endl;
            //                std::cerr << insert_day << std::endl;
        }
        std::cerr << "code: " << filename.substr(11, 4)/*code*/ << std::endl;
    }

    sqlite3_exec(db, "COMMIT", NULL, NULL, NULL);
    sqlite3_close(db);
    system("rm tmp_filename");
}
