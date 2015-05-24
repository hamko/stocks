#include "StockData.hpp"
#include <iostream>
#include <sqlite3.h>

void StockData::addStockDay(std::string code, std::string market, std::string name, std::string category, StockDay* sd)
{
    if (!stock_.count(code)) {
        stock_[code] = new StockDayStream(code, market, name, category); 
    }
    stock_[code]->addStockDay(sd);
}

StockDayStream* StockData::getStockDayStream(std::string code)
{
    if (!stock_.count(code)) {
        std::cerr << "No such code" << " " << __FILE__ << " " << __LINE__ << std::endl;
    }

    return stock_[code];
}

void StockData::sortAll(void)
{
    for (std::map<std::string, StockDayStream*>::iterator it = stock_.begin(); it != stock_.end(); it++) {
//        std::cerr << it->second->getCode() << std::endl;
        it->second->sort();
    }
}

StockData::~StockData(void)
{
    for (std::map<std::string, StockDayStream*>::iterator it = stock_.begin(); it != stock_.end(); it++) {
        delete it->second;
    }
}

// 抽出結果が返るコールバック関数
static int callback(void *NotUsed, int argc, char **argv, char **azColName){
    int i;
    for(i=0; i<argc; i++)
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    return SQLITE_OK;
}

int CountCallback( void* pOutCount, int size, char **rec, char **ColName ){
       int *count = (int*)pOutCount;
          *count = atoi( *rec );
             return 0;
}
void StockData::save(void)
{
    sqlite3 *db;
    char *zErrMsg = 0;

    // データベースファイルを新規生成
    int rc = sqlite3_open("Sample.db", &db);

    // テーブル生成SQL文
    char create_sql1[] = "CREATE TABLE t_stock ( "
        "               code   TEXT PRIMARY KEY, "
        "               name   TEXT    "
        "             )                             ";

    // テーブルを生成
    rc = sqlite3_exec(db, create_sql1, 0, 0, &zErrMsg);
//    std::cerr << zErrMsg << std::endl;

    char create_sql2[] = "CREATE TABLE t_price ( "
        "               id   TEXT PRIMARY KEY, "
        "               stock_code TEXT REFERENCES t_stock(code),    "
        "               time REAL,    "
        "               prices REAL,    "
        "               lowest REAL,    "
        "               highest REAL,    "
        "               opening REAL,    "
        "               closing REAL,    "
        "               value REAL,    "
        "               volume REAL    "
        "             )                             ";

    // テーブルを生成
    rc = sqlite3_exec(db, create_sql2, 0, 0, &zErrMsg);
//    std::cerr << zErrMsg << std::endl;

    int price_id = 0;
    sqlite3_exec(db, "BEGIN", NULL, NULL, NULL);
    for (std::map<std::string, StockDayStream*>::iterator it = stock_.begin(); it != stock_.end(); it++) {
        // データ追加SQL文
        char insert_stock_templete[] = "INSERT INTO t_stock(code, name)"
            "                values ('%s', '%s')     ";
        char insert_stock[1024];
        sprintf(insert_stock, insert_stock_templete, it->second->getCode().c_str(), it->second->getName().c_str());
        rc = sqlite3_exec(db, insert_stock, 0, 0, &zErrMsg);
        for (stock_itr itd = it->second->begin(); itd != it->second->end(); itd++) {
            // データ追加SQL文
            char insert_day_templete[] = "INSERT INTO t_price(id, stock_code, time, prices, lowest, highest, opening, closing, value, volume)"
                "                values (%d, '%s', %d, %d, %d, %d, %d, %d, %d, %d)";
            char insert_day[1024];
            sprintf(insert_day, insert_day_templete, price_id, it->second->getCode().c_str(), (*itd)->getDay(), (*itd)->getPrices(), (*itd)->getLowest(), (*itd)->getHighest(), (*itd)->getOpening(), (*itd)->getClosing(), (*itd)->getValue(), (*itd)->getVolume());
            rc = sqlite3_exec(db, insert_day, 0, 0, &zErrMsg);
//            std::cerr << zErrMsg << std::endl;

            price_id++;
        }
//        std::cerr << "code: " << it->second->getCode().c_str() << std::endl;
    }
    sqlite3_exec(db, "COMMIT", NULL, NULL, NULL);

    // デバッグ用レコードカウント
    char count_SQL[] = "SELECT COUNT(*) FROM t_price";
    int iCount = 0;
    sqlite3_exec( db, count_SQL, CountCallback, &iCount, &zErrMsg );
//    std::cerr << zErrMsg << std::endl;
//    std::cout << iCount << std::endl;

    // 列挙
    rc = sqlite3_exec(db, "SELECT day FROM t_price", callback, 0, &zErrMsg);

    // データベースを閉じる
    sqlite3_close(db);
}
