#include <iostream>
#include "StockData.hpp"
#include "Loader.hpp"
#include "k_db_com_loader.hpp"
#include "WallStreet_loader.hpp"
#include "DataWarehouse_loader.hpp"
#include "decisionCross.hpp"
#include "YahooStocksRealtime.hpp"
using namespace std;

int main(void)
{
    // Yahoo Financeからリアルタイム株価を取得
    while (1) {
        time_t now = time(NULL);
        struct tm *pnow = localtime(&now);
        cout << pnow->tm_hour << " " << pnow->tm_min << " " << pnow->tm_sec << " " << YahooStocksRealtime(8715, "t").getPrices() << endl; 
        sleep(10);
    }

    // 準備
#if 0
    StockData* data = new StockData();
    //   Loader* loader = new k_db_com_loader();
    //    Loader* loader = new WallStreet_loader();
    Loader* loader = new DataWarehouse_loader();
    loader->load(data);
    cout << "# Loaded." << endl;    

    // データベース化
    data->save();

    // 処理
    StockDayStream* ganhoo = data->getStockDayStream("3765");
    decisionCross* dc = new decisionCross(ganhoo, 5, 16, 5);
    for (stock_itr it = ganhoo->begin(); it != ganhoo->end(); it++) {
        //        cout << (*it)->getDay() << endl;    
        dc->update(it);
    }

    // 削除
    delete data;
    delete loader;
#endif

    return 0; 

}
