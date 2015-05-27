#include <iostream>
#include "Loader.hpp"
#include "k_db_com_loader.hpp"
#include "WallStreet_loader.hpp"
#include "DataWarehouse_loader.hpp"
#include "decisionCross.hpp"
#include "YahooStocksRealtime.hpp"
#include "StockDayStreamAllocator.hpp"

#include <hamkoutil.hpp> 

using namespace std;

int main(int argc, char** argv)
{
    if (argc < 2) {
        cerr << "usage: ./a.out [sqlite3 database]" << endl; 
        return 1;
    }

    char* database = argv[1];
    int load_flag = 0;
    if (argc > 2 && !strcmp(argv[2], "-new")) {
        load_flag = 1; 
    }


    // Yahoo Financeからリアルタイム株価を取得
    /*
    stopwatch_start(0);
    for (int i = 0; i < 10000; i++) {
        time_t now = time(NULL);
        struct tm *pnow = localtime(&now);
        int prices = YahooStocksRealtime(i, "t").getPrices();
        if (prices >= 0)
            cout << i << "\t" << prices << "\t" << pnow->tm_hour << "\t" << pnow->tm_min << "\t" << pnow->tm_sec << endl; 
    }
    cout << "#" << stopwatch_end(0) << endl;
    */


    // 準備
    //   Loader* loader = new k_db_com_loader();
    //    Loader* loader = new WallStreet_loader();
    if (load_flag) {
        Loader* loader = new DataWarehouse_loader();
        loader->load(database);
        delete loader;
        cout << "# Loaded." << endl;    
    }

    // 処理
    string database_string(database);
    StockDayStreamAllocator* allocator= new StockDayStreamAllocator(database_string);
    StockDayStream* ganhoo = allocator->newStockDayStream("3765", "2014-12-01", "2014-12-15");
    decisionCross* dc = new decisionCross(ganhoo, 5, 16, 5);
    for (stock_itr it = ganhoo->begin(); it != ganhoo->end(); it++) {
        //        cout << (*it)->getDay() << endl;    
        dc->update(it);
    }


    return 0; 

}
