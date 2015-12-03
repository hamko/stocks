#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include "StockMinute.hpp"

StockMinute::StockMinute(int code, string name) 
    : m_code(code), m_name(name)
{
}

StockMinute::StockMinute(int code, string name, string filename) 
    : m_code(code), m_name(name)
{
    std::ifstream ifs(filename);
    std::string str;
    int t = 0;
    while(getline(ifs,str))
    {
        string data[8];
        string tmp;
        std::istringstream stream(str);
        int i = 0;
        while(getline(stream,tmp,','))
        {
            data[i] = tmp;
            i++;
        }
        this->m_day.push_back(data[0]);
        this->m_time.push_back(data[1]);
        this->m_minute.push_back(t);

        if (data[2].find("-")) 
            this->m_hajimene.push_back(stod(data[2]));
        else 
            this->m_hajimene.push_back(NAN);

        if (data[3].find("-")) 
            this->m_takane.push_back(stod(data[3]));
        else 
            this->m_takane.push_back(NAN);

        if (data[4].find("-")) 
            this->m_yasune.push_back(stod(data[4]));
        else 
            this->m_yasune.push_back(NAN);

        if (data[5].find("-")) 
            this->m_owarine.push_back(stod(data[5]));
        else 
            this->m_owarine.push_back(NAN);

        this->m_volume.push_back(stod(data[6]));
        this->m_volume_price.push_back(stod(data[7]));
        t++;
    }
    reverse(this->m_hajimene.begin(), this->m_hajimene.end());
    reverse(this->m_owarine.begin(), this->m_owarine.end());
    reverse(this->m_yasune.begin(), this->m_yasune.end());
    reverse(this->m_takane.begin(), this->m_takane.end());
    reverse(this->m_volume.begin(), this->m_volume.end());
    reverse(this->m_volume_price.begin(), this->m_volume_price.end());
}

void StockMinute::addData(double time, double owarine)
{
    m_minute.push_back(time);
    m_owarine.push_back(owarine);
}

int StockMinute::size(void)
{
    return m_minute.size();
}
