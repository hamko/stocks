#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "Agent.hpp"
#include "StockMinute.hpp"
#include "leasqr_line.hpp"
#include "stopwatch_util.hpp"
using namespace std;

#ifndef TEST
int main(void)
{
//    vector<string> filenames =  {"../7203/20150930"};
    vector<string> filenames =  {"../7203/20150818", "../7203/20150907", "../7203/20150930", "../7203/20151022",  "../7203/20150819", "../7203/20150908", "../7203/20151001", "../7203/20151023", "../7203/20150820", "../7203/20150909", "../7203/20151002", "../7203/20151026", "../7203/20150821", "../7203/20150910", "../7203/20151005", "../7203/20151027", "../7203/20150824", "../7203/20150911", "../7203/20151006", "../7203/20151028", "../7203/20150825", "../7203/20150914", "../7203/20151007", "../7203/20151029", "../7203/20150826", "../7203/20150915", "../7203/20151008", "../7203/20151030", "../7203/20150827", "../7203/20150916", "../7203/20151009", "../7203/20151102", "../7203/20150828", "../7203/20150917", "../7203/20151013", "../7203/20151104", "../7203/20150831", "../7203/20150918", "../7203/20151014", "../7203/20151105", "../7203/20150901", "../7203/20150924", "../7203/20151015", "../7203/20151106", "../7203/20150902", "../7203/20150925", "../7203/20151016", "../7203/20151109", "../7203/20150903", "../7203/20150928", "../7203/20151020", "../7203/20151110", "../7203/20150904", "../7203/20150929", "../7203/20151021"};


    stopwatch_start(0);

    // parameters
    vector<double> thres_test_range = {.1,.2,.3,.4,.5,.6};
    vector<double> thres_sl_range = {10,20,30,40,50,60,70,80,90,100};
//    vector<double> thres_test_range = {.1};
//    vector<double> thres_sl_range = {10};
    for (int thres_test_i = 0; (size_t)thres_test_i < thres_test_range.size(); thres_test_i++) {
    double thres_test = thres_test_range[thres_test_i];
    cerr << thres_test_i+1 << "/" << thres_test_range.size() << "\r" << endl;
    for (int thres_sl_i = 0; (size_t)thres_sl_i < thres_sl_range.size(); thres_sl_i++) {
    double thres_sl = thres_sl_range[thres_sl_i];


    double sum_profit = 0;
    int sum_trade_num = 0;
#pragma omp parallel for
    for (int i = 0; i < (int)filenames.size(); i++) {
        int code = 7203;
        int minimum_unit = 300;
        Agent agent(3000000);
        agent.addHoldingStock(new HoldingStock(code));
        StockMinute sm(code, "Toyota", filenames[i]);
        double a_l_prev = NAN, b_l_prev = NAN, a_u_prev = NAN, b_u_prev = NAN;
        for (int t = 0; t < 360; t++) {
            double price = sm.m_owarine[t];
            if (std::isnan(price))
                continue;
            int len = 30;
            if (t >= len) {
                int ret;
                double a_l, b_l;
                ret = leasqr_line(sm.m_minute, sm.m_owarine, t-30, len, LEASQR_LINE_LOWER, 10, &a_l, &b_l);
                if (ret) continue;
                double a_u, b_u;
                ret = leasqr_line(sm.m_minute, sm.m_owarine, t-30, len, LEASQR_LINE_UPPER, 10, &a_u, &b_u);
                if (ret) continue;

                if (a_u_prev-a_l_prev < thres_test && a_u - a_l > thres_test) {
                    agent.sell(code, t, minimum_unit, price, price+thres_sl, price-60);
                }
                if (a_u_prev-a_l_prev > -thres_test && a_u - a_l < -thres_test) {
                    agent.buy(code, t, minimum_unit, price, price-thres_sl, price+60);
                }

                a_u_prev = a_u; a_l_prev = a_l;
                b_u_prev = b_u; b_l_prev = b_l;
            }

            agent.tradeBySLTP(code, t, price);

//            cout << agent.getHoldingStock(7203)->m_num << " " <<  agent.getHoldingStock(7203)->m_price << endl;
        }

        double last_price = sm.m_owarine[sm.m_owarine.size()-1];
        agent.ForcedSettlement(code, sm.m_owarine.size()-1, last_price);
        sum_profit += agent.getProfit();
        sum_trade_num += agent.getTradeNum();
    }
    cout << sum_profit << " " << sum_trade_num << " " << sum_profit / sum_trade_num<< " " << thres_test << " " << thres_sl << endl;

    }} // parameters

    double time = stopwatch_end(0);
    //    cout << time << " #endtime" << endl;

    return 0;
}
#endif //TEST
