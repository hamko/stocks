#include <sqlite3.h>
#include <iostream>
#include <string>

void InitDatabase(std::string database)
{
    sqlite3 *db;
    char *zErrMsg = 0;

    // データベースファイルを新規生成
    int rc = sqlite3_open(database.c_str(), &db);

    // テーブル生成SQL文
    char create_sql1[] = "CREATE TABLE t_stock ( "
        "               code   TEXT PRIMARY KEY, "
        "               name   TEXT    "
        "             )                             ";

    // テーブルを生成
    rc = sqlite3_exec(db, create_sql1, 0, 0, &zErrMsg);
//    std::cerr << zErrMsg << std::endl;

    char create_sql2[] = "CREATE TABLE t_price ( "
        "               stock_code TEXT REFERENCES t_stock(code),    "
        "               time TIMESTAMP PRIMARY KEY,    "
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
    std::cerr << zErrMsg << std::endl;

    // 列挙
//    rc = sqlite3_exec(db, "select name from sqlite_master where type = 'table'", callback, 0, &zErrMsg);

    // データベースを閉じる
    sqlite3_close(db);
}
