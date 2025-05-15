#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <iomanip>

#include "DataStructures.h"

typedef std::string string;


Time_Stamp::Time_Stamp(int yyyy, int mm, int dd)
     : YYYY(yyyy), MM(mm), DD(dd) {
}

Time_Stamp::Time_Stamp(int yyyy, int mm, int dd, int hour, int min, int sec)
    : YYYY(yyyy), MM(mm), DD(dd), Hour(hour), Min(min), Sec(sec) {
}

Time_Stamp::Time_Stamp(string timeStampAsString)
{
    std::cout << "TimeStamp created from string: " << timeStampAsString << "\n";

    int timeStampFormat = 0;
    if (timeStampAsString.length() == 10)
    {
        timeStampFormat = sscanf_s(timeStampAsString.c_str(), "%d-%d-%d", &YYYY, &MM, &DD);
        Hour = 0;
        Min = 0;
        Sec = 0;
    }

    else if (timeStampAsString.length() == 19)
    {
        timeStampFormat = (sscanf_s(timeStampAsString.c_str(), "%d-%d-%d %d:%d:%d", &YYYY, &MM, &DD, &Hour, &Min, &Sec));
    }

    else
    {
        std::cout << "INVALID TIME STAMP FORMAT. String length is " << timeStampAsString.length() << "\n";
        YYYY = 0;
        MM = 0;
        DD = 0;
        Hour = 0;
        Min = 0;
        Sec = 0;
    }
}

string Time_Stamp::ToString()
{
    std::ostringstream oss;
    oss << YYYY 
        << "-" << std::setw(2) << std::setfill('0') <<MM 
        << "-" << std::setw(2) << std::setfill('0') << DD;

    if (Hour != 0 || Min != 0 || Sec != 0)
        oss << " " << std::setw(2) << std::setfill('0') << Hour 
            << ":" << std::setw(2) << std::setfill('0') << Min
            << ":" << std::setw(2) << std::setfill('0') << Sec;

    return oss.str();
}


Candle::Candle(Time_Stamp timeStamp, float open, float high, float low, float close, float volume)
    : TimeStamp(timeStamp), Open(open), High(high), Low(low), Close(close), Volume(volume) {
}

Candle DataSet::operator[](int i)
{
    return Candles[i];
}

int DataSet::size()
{
    return Candles.size();
}

void StrategyReport::WriteToFile(string path, bool includeHeader)
{
    std::ofstream ofs(path);

    if (includeHeader)
    {
        //ofs << "Symbol = " << Symbol << "\n";
        ofs << "Strategy = " << StrategyID << "\n";
        ofs << "Total Percentage Growth = " << GrowthPercentTotal << "\%\n";
        ofs << "------------------------------\n";
    }

    float dataCount = TimeStamps.size();

    ofs << "TimeStamp, Equity Curve, Equity Delta, Fraction Invested \n";
    for (int i = 0; i < dataCount; i++)
        ofs << TimeStamps[i].ToString() << "," << EquityCurve[i] << "," << InvestmentDelta[i] << "," << FractionInvested[i] << "\n";
    
    ofs.close();
}