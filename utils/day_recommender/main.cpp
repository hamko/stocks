#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <ctime>
#include "Agent.hpp"
#include "StockMinute.hpp"
#include "leasqr_line.hpp"
#include "YahooStocksRealtime.hpp"
using namespace std;

#ifndef TEST
int main(void)
{ 
    double thres_test = 0.3;
    double thres_sl = 30;

    int code = 7203;
    int minimum_unit = 300;
    Agent agent(3000000);
    agent.addHoldingStock(new HoldingStock(code));
    StockMinute sm(code, "Toyota");

    double a_l_prev = NAN, b_l_prev = NAN, a_u_prev = NAN, b_u_prev = NAN;
    // TODO ここに日々きちんと動かすための処理
    for (int t = 0; t < 360; t++) {
        int hour = 9, minute = 0;
        int hour_end = 15, minute_end = 0;
        time_t now; struct tm *pnow; now = time(NULL); pnow = localtime(&now);
        while ((pnow->tm_hour-hour)*60+(pnow->tm_min-minute) < t) {
            time_t now = time(NULL); struct tm *pnow = localtime(&now);
            sleep(1);
        }
        YahooStocksRealtime pricegetter = YahooStocksRealtime(7203, "t");
        double price = pricegetter.m_price;
        sm.addData(t, price);
        if (std::isnan(price))
            continue;

        int len = 5; 
        if (sm.size() >= len) {
            int ret;
            double a_l, b_l;
            ret = leasqr_line(sm.m_minute, sm.m_owarine, t-len, len, LEASQR_LINE_LOWER, 10, &a_l, &b_l);
            if (ret) continue;
            double a_u, b_u;
            ret = leasqr_line(sm.m_minute, sm.m_owarine, t-len, len, LEASQR_LINE_UPPER, 10, &a_u, &b_u);
            if (ret) continue;

            if (a_u_prev-a_l_prev < thres_test && a_u - a_l > thres_test) {
                agent.buy(code, t, minimum_unit, price, price-thres_sl, price+60);
                system("echo トヨタを買ってください！ブレイクアウトしました！日々乃でした！>> tmp");
                system("cat tmp | mail -s SELL wakataberyo@gmail.com");
                system("rm tmp");
            }
            if (a_u_prev-a_l_prev > -thres_test && a_u - a_l < -thres_test) {
                agent.sell(code, t, minimum_unit, price, price+thres_sl, price-60);
                system("echo トヨタを売ってください！ブレイクアウトしました！日々乃でした！>> tmp");
                system("cat tmp | mail -s SELL wakataberyo@gmail.com");
                system("rm tmp");
            }

            a_u_prev = a_u; a_l_prev = a_l;
            b_u_prev = b_u; b_l_prev = b_l;
        } else {
            system("echo 日々乃ひいなです！データが足りないので解析できません！ >> tmp");
            system("cat tmp | mail -s test wakataberyo@gmail.com");
            system("rm tmp");
        }

        if (agent.tradeBySLTP(code, t, price) != 0) {
            system("echo ポジション手じまってください！損切りもしくは利食い発生です！日々乃でした！ >> tmp");
            system("cat tmp | mail -s test wakataberyo@gmail.com");
            system("rm tmp");
        }
    }
    double last_price = sm.m_owarine[sm.m_owarine.size()-1];
    if (agent.ForcedSettlement(code, sm.m_owarine.size()-1, last_price) != 0) {
            system("echo ポジション手じまってください！一日の終わりの手仕舞いです！日々乃でした！ >> tmp");
            system("cat tmp | mail -s test wakataberyo@gmail.com");
            system("rm tmp");
    }

    return 0;
}
#endif //TEST
