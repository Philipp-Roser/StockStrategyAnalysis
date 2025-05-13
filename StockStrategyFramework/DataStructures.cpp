#include <iostream>
#include <sstream>
#include <string>

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
        std::cout << "INVALID TIME STAMP FORMAT. String length is " << timeStampAsString.length();
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
    oss << YYYY << "-" << MM << "-" << DD;

    if (Hour != 0 || Min != 0 || Sec != 0)
        oss << " " << Hour << ":" << Min << ":" << Sec;

    return oss.str();
}


Candle::Candle(Time_Stamp timeStamp, float open, float high, float low, float close, float volume)
    : TimeStamp(timeStamp), Open(open), High(high), Low(low), Close(close), Volume(volume) {
}
