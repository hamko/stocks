#include "StockDayStreamAllocator.hpp"
#include <iostream>
#include <sqlite3.h>
#include <string>

// 抽出結果が返るコールバック関数
static int callback(void* stream, int argc, char **argv, char **azColName){
    if (argc != 6) {
        std::cerr << "wrong argc" << std::endl;
    }
    static_cast<StockDayStream*>(stream)->addStockDay(new StockDay(std::string(argv[0]), atof(argv[1]), atof(argv[2]), atof(argv[3]), atof(argv[4]), atof(argv[5]), -1));

    return SQLITE_OK;
}

StockDayStream* StockDayStreamAllocator::newStockDayStream(std::string code, std::string day_begin, std::string day_end)
{
    StockDayStream* stream = new StockDayStream();
    // 本当はcodeとかnameとかを読みだしてStockDayStreamを更新 TODO
    // データベースにアクセスしてbeginからendまでのコールバックでひたすらデータ読み出し
    sqlite3* db;
    char* zErrMsg;
    sqlite3_open(database.c_str(), &db);
    std::string command = "select stock_code, time, prices, lowest, highest, opening, closing, value from t_price";
    sqlite3_exec(db, command.c_str(), callback, stream, &zErrMsg);
    if (zErrMsg) {
        std::cerr << zErrMsg << std::endl;
    }
    sqlite3_close(db);

    return stream;
}
StockDayStream* StockDayStreamAllocator::newStockDayMinStream(std::string code, std::string day)
{
    return NULL; // TODO
}

