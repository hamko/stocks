#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <sstream>
#include <ctime>
#include "Agent.hpp"
#include "StockMinute.hpp"
#include "leasqr_line.hpp"
#include "YahooStocksRealtime.hpp"
using namespace std;

double thres_test = 0.3;
double thres_sl = 60;
int len = 80; 
int hour = 9, minute = 0;
int hour_end = 15, minute_end = 0;
void waitOpening(void)
{
    while (1) {
        time_t now = time(NULL); struct tm *pnow = localtime(&now);
        if (pnow->tm_hour < hour || pnow->tm_hour >= hour_end) {
            sleep(1);
        } else {
            break;
        }
    }
}

void mail(double profit, string comment, string title)
{
    std::ostringstream os0; 
    os0 << "echo " << comment << " >> tmp";
    system(os0.str().c_str());

    if (profit) {
        std::ostringstream os1; 
        os1 << "echo 今日は全部で" << (int)profit << "円の儲けですね！>> tmp";
        system(os1.str().c_str());
    }
    system("echo 日々乃でした！ >> tmp");

    std::ostringstream os2; 
    os2 << "cat tmp | mail -s " << title << " wakataberyo@gmail.com";
    system(os2.str().c_str());

    system("rm tmp");

    cout << profit << " " << comment << " " << title << "#mail" << endl;
}


#ifndef TEST
int main(void)
{ 
    int code = 7203;
    int minimum_unit = 400;
    Agent agent(4000000);
    agent.addHoldingStock(new HoldingStock(code));
    StockMinute sm(code, "Toyota");

    double a_l_prev = NAN, b_l_prev = NAN, a_u_prev = NAN, b_u_prev = NAN;
    while (1) { // days
    waitOpening();
    for (int t = 0; t < 360; t++) {
        if (t == 0) {
            mail(agent.getProfit(), "今日の取引を始めます！", "Opening");
        }

        while (1) {
            time_t now = time(NULL); struct tm *pnow = localtime(&now);
            if ((pnow->tm_hour-hour)*60+(pnow->tm_min-minute) < t) {
                sleep(1);
            } else {
                break;
            }
        }
        YahooStocksRealtime pricegetter = YahooStocksRealtime(7203, "t");
        double price = pricegetter.m_price;
        sm.addData(t, price);
        if (std::isnan(price))
            continue;

        if (sm.size() >= len) {
            int ret;
            double a_l, b_l;
            ret = leasqr_line(sm.m_minute, sm.m_owarine, t-len, len, LEASQR_LINE_LOWER, 10, &a_l, &b_l);
            if (ret) continue;
            double a_u, b_u;
            ret = leasqr_line(sm.m_minute, sm.m_owarine, t-len, len, LEASQR_LINE_UPPER, 10, &a_u, &b_u);
            if (ret) continue;

/*
            if (t % 3 == 0)  {
                std::ostringstream os; 
		os << "echo " << a_u << " " << b_u << " >> tmp";
                system(os.str().c_str());
                system("cat tmp | mail -s SELL wakataberyo@gmail.com");
                system("rm tmp");
            }
*/

            if (a_u_prev-a_l_prev < thres_test && a_u - a_l > thres_test) {
                int failed = agent.buy(code, t, minimum_unit, price, price-thres_sl, price+60);
                if (!failed) {
		    mail(agent.getProfit(), "トヨタを買ってください！ブレイクアウトしました！", "Buy");
                }
            }

            if (a_u_prev-a_l_prev > -thres_test && a_u - a_l < -thres_test) {
                int failed = agent.sell(code, t, minimum_unit, price, price+thres_sl, price-60);
                if (!failed) {
		    mail(agent.getProfit(), "トヨタを売ってください！ブレイクアウトしました！", "Sell");
                }
            }

            a_u_prev = a_u; a_l_prev = a_l;
            b_u_prev = b_u; b_l_prev = b_l;
        }

        if (agent.tradeBySLTP(code, t, price) != 0) {
	    mail(agent.getProfit(), "ポジション手じまってください！損切りもしくは利食い発生です！", "SLTP");
        }

        cout << price << " " << agent.getProfit() << " " << agent.getUsingConsignmentGuaranteeMoney() << " " << agent.getHoldingStock(7203)->m_num << " " << agent.getHoldingStock(7203)->getNeededConsignmentGuaranteeMoney() << " " << agent.getHoldingStock(7203)->m_price << "#debug" << endl;
    }
    double last_price = sm.m_owarine[sm.m_owarine.size()-1];
    if (agent.ForcedSettlement(code, sm.m_owarine.size()-1, last_price) != 0) {
        mail(agent.getProfit(), "ポジション手じまってください！一日の終わりの手仕舞いです！", "ClosingTrade");
    }
    mail(agent.getProfit(), "今日の取引は終わりです！", "Closing");


    sleep(3600); // to make sure closing
    waitOpening();
    }

    return 0;
}
#endif //TEST
