#include "decisionCross.hpp"
#include <iostream>
#include <cmath>
#include <vector>

decisionCross::decisionCross(StockDayStream* stock, int s, int m, int a)
    : decision(stock), short_term_tau(s), medium_term_tau(m), analysis_period(a)
{
}

void decisionCross::update(stock_itr day)
{
    if (day - stock_->begin() <= medium_term_tau + analysis_period) {
//        std::cout << "not enough length" << __FILE__ << " " << __LINE__<< std::endl;
        return;
    }

    std::vector<double> medium_precision, short_precision;
    for (stock_itr it = day - analysis_period; it != day; it++) {
        double m = 0.0;
        for (stock_itr itj = it - medium_term_tau; itj != it; itj++) 
            m += (*itj)->getClosing();
        medium_precision.push_back(m / medium_term_tau);
        double s = 0.0;
        for (stock_itr itj = it - short_term_tau; itj != it; itj++) 
            s += (*itj)->getClosing();
        short_precision.push_back(s / short_term_tau);
    }

    std::vector<double> diff(medium_precision.size());
    for (int i = 0; i < (int)diff.size(); i++) 
        diff.at(i) = short_precision.at(i) - medium_precision.at(i);

    tau_ = short_term_tau;
    // 並行ならエラー
    if (diff.at(diff.size()-1)-diff.at(0) == 0) {
        buy_ = 0;
        credibility_ = 0;
        return;
    }
    double cross = -diff.at(diff.size()-1)/((diff.at(diff.size()-1)-diff.at(0))/analysis_period); // 最終値から何日後にクロスが出そうか
    buy_ = (-analysis_period < cross && cross < 0/*TODO クロスが出てからじゃないと買わないことになってる*/) * (diff.at(0)>0?1:-1); // 解析期間中にクロスが出ていれば購入サインor売却サイン
    credibility_ = (diff.at(diff.size()-1)-diff.at(0))/analysis_period; // クロスの強さも参考データとして返す．

    std::cout << (*day)->getDay() << " " << (*day)->getClosing() << " " << buy_ << " " << credibility_ << " " << cross << " " << medium_precision.at(medium_precision.size()-1) << " " << short_precision.at(short_precision.size()-1) << std::endl;
}
