#pragma once


struct Time_Stamp
{
    int YYYY;
    int MM;
    int DD;
    int Hour = 0;
    int Min = 0;
    int Sec = 0;

    Time_Stamp(int yyyy, int mm, int dd);

    Time_Stamp(int yyyy, int mm, int dd, int hour, int min, int sec);

    Time_Stamp(std::string timeStampAsString);

    std::string ToString();
};

struct Candle
{
    Time_Stamp TimeStamp;

    float Open;
    float High;
    float Low;
    float Close;
    float Volume;

    Candle(Time_Stamp timeStamp, float open, float high, float low, float close, float volume);
};