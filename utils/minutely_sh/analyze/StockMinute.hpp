#pragma once
#include <vector>
#include <string>
using namespace std;

class StockMinute {
public:
    int m_code;
    string m_name;
    vector<string> m_day;
    vector<string> m_time;
    vector<double> m_minute;
    vector<double> m_hajimene;
    vector<double> m_takane;
    vector<double> m_yasune;
    vector<double> m_owarine;
    vector<double> m_volume;
    vector<double> m_volume_price;
    StockMinute(int code, string name, string filename);
};
