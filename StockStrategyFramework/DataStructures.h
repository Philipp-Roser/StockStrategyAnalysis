#pragma once

#include <vector>

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

struct DataSet
{
    std::string Symbol = "";
    std::vector<Candle> Candles;
        
    Candle operator[](int i);
    int size();

};


enum Buy_Or_Sell { buy = 1, sell = -1 };

struct BuySellInstruction
{
    Buy_Or_Sell BuyOrSell = buy;
    float Amount = 0;
    std::string TypedInstruction();
};


struct StrategyReport
{
    std::string StrategyID = "";
    
    float GrowthPercentTotal = 0;
    float MarketGrowthTotal = 0;
    float RelativeVolatility = 0;
    
    std::vector<Time_Stamp> TimeStamps;
    std::vector<float> StockPriceAtOpen;
    std::vector<float> EquityCurve;
    std::vector<BuySellInstruction> Instruction;

    void WriteToFile(std::string path, bool includeHeader = true);
    void CalculateVolatility();
};