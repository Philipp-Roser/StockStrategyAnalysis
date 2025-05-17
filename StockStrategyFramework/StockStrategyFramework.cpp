#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>



#include "DataStructures.h"
#include "StockStrategyFramework.h"

using string = std::string;



/*
 TO DO : Add Bid-ask spread
*/

DataSet DataSetFromCSV(string csvPath, bool isInReverseChronologicalOrder)
{
    std::ifstream file(csvPath);
    DataSet dataSet;
    
    string headerLine;
    std::getline(file, headerLine);
    if (headerLine != "timestamp,open,high,low,close,volume")
    {
        std::cout << "First line is:\n" << headerLine << "\n but should read: \"timestamp,open,high,low,close,volume\".\n";
        return dataSet;
    }

    int lineNumber = 1;
    std::string thisLine;
    while (std::getline(file, thisLine))
    {
        std::vector<string> fields;
        std::istringstream stream(thisLine);
        std::string thisField;

        while (std::getline(stream, thisField, ','))
        {
            fields.push_back(thisField); 
        }

        if (fields.size() != 6)
        {
            std::cout << "ERROR: Line " << lineNumber << " does not have the correct number of fields. Terminating in the previous line.";
            return dataSet;
        }

        Time_Stamp timeStamp = Time_Stamp(fields[0]);
        float open = std::stof(fields[1]);
        float high = std::stof(fields[2]);
        float low = std::stof(fields[3]);
        float close = std::stof(fields[4]);
        float volume = std::stof(fields[5]);

        dataSet.Candles.push_back(Candle(timeStamp, open, high, low, close, volume));
    }

    if(isInReverseChronologicalOrder)
        std::reverse(dataSet.Candles.begin(), dataSet.Candles.end());
    
        
    std::cout << "Created data set with " << dataSet.size() << " candles.\n";
    std::cout << "Date starts at " << dataSet[0].TimeStamp.ToString() << "\n";
    return dataSet;
    

}



StrategyReport ExecuteStrategy( string strategyID, 
                                Strategy strategy, 
                                DataSet* dataSet, 
                                float totalInitialEquity, 
                                float fractionInitiallyInvested)
{
    std::cout << "Executing strategy ...\n";
    StrategyReport report;
    int dataCount = dataSet->size();

    report.StrategyID = strategyID;
    report.EquityCurve.reserve(dataCount);
    report.StockPriceAtOpen.reserve(dataCount);
    report.Instruction.reserve(dataCount);

    float totalEquity = totalInitialEquity;
    float investment = totalEquity * fractionInitiallyInvested;
    float cash = totalEquity - investment;

    for (int i = 0; i < dataCount; i++)
    {
        report.TimeStamps.push_back(dataSet->Candles[i].TimeStamp.ToString());
        report.EquityCurve.push_back(cash + investment);
        report.StockPriceAtOpen.push_back((*dataSet)[i].Open);

        BuySellInstruction instruction = strategy(dataSet, i, cash, investment);
        
        report.Instruction.push_back(instruction);

        switch (instruction.BuyOrSell)
        {
        case buy:
            cash -= instruction.Amount;
            investment += instruction.Amount;
            break;

        case sell:
            cash += instruction.Amount;
            investment -= instruction.Amount;
            break;
        }

        if (i == dataCount - 1)
            break;

        // Advance to next candle:
        float thisAssetPrice = dataSet->Candles[i].Open;
        float nextAssetPrice = dataSet->Candles[i + 1].Open;
        investment *= nextAssetPrice / thisAssetPrice;
        
    }


    report.GrowthPercentTotal = 100 * ((investment + cash) / totalInitialEquity - 1);
    report.CalculateVolatility();

    report.MarketGrowthTotal = 100 * (dataSet->Candles.back().Close / dataSet->Candles.front().Open - 1);

    std::cout << "Strategy executed.\n";

    return report;
}



StrategiesMetaReport ExecuteStrategyCollection(
    std::vector<std::string> strategyIDs,
    std::vector<Strategy> strategies,
    DataSet* dataSet,
    float totalInitialEquity,
    float totalInitiallyInvested)
{
    if (strategyIDs.size() != strategies.size())
    {
        std::cout << "ERROR: stategyIDs vs strategies count mismatch.";
        StrategiesMetaReport report;
        return report;
    }

    int strategyCount = strategies.size();
    std::vector<StrategyReport> stratReports;

    StrategyReport currentBest;

    for (int i = 0; i < strategyCount; i++)
    {
        StrategyReport thisStrategyReport = ExecuteStrategy(
            strategyIDs[i], strategies[i], dataSet, totalInitialEquity, totalInitiallyInvested);
                
        stratReports.push_back(thisStrategyReport);
        if (thisStrategyReport.GrowthPercentTotal > currentBest.GrowthPercentTotal)
            currentBest = thisStrategyReport;
    }

    StrategiesMetaReport metaReport;
    metaReport.StategyReports = stratReports;
    metaReport.BestStrategy = currentBest;
    metaReport.LargestGrowth = currentBest.GrowthPercentTotal;

    return metaReport;
}
