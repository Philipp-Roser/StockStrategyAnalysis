#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include <windows.h> // TEMP


#include "DataStructures.h"



using string = std::string;
using Strategy = BuySellInstruction(*)(DataSet* dataSet, int i, float cashOnHand, float currentlyInvested);


// forward declarations:
DataSet DataSetFromCSV(string csvPath, bool isInReverseChronologicalOrder = true);
StrategyReport ExecuteStrategy(string strategyID,
    Strategy strategy,
    DataSet* dataSet,
    float totalInitialEquity = 1000,
    float fractionInitiallyInvested = 0.5);

BuySellInstruction TestStrategy(DataSet* dataSet, int i, float cashOnHand, float currentlyInvested);


int main()
{
    // Test code

    std::cout << "Test\n";


    char buffer[MAX_PATH];
    DWORD length = GetModuleFileNameA(NULL, buffer, MAX_PATH);
    string path = std::string(buffer, length);
    size_t lastSlash = path.find_last_of("\\/");
    path = path.substr(0, lastSlash);
    path.append("/VTI_Daily.csv");
    std::cout << "Filepath: " << path << "\n";

    DataSet testDataSet = DataSetFromCSV(path);
    StrategyReport testReport = ExecuteStrategy("TestStrat", TestStrategy, &testDataSet, 1000, 0.1);
    
    std::cout /* << testReport.Symbol*/ << "\n" << testReport.StrategyID << "\n" << testReport.GrowthPercentTotal << "\n\n";
    std::cout << "Date \t\t\ Equity curve: \t\tFractionInvested  \n";

    for (int i = 1; i < testReport.EquityCurve.size(); i++)
    {
        std::cout << testReport.TimeStamps[i].ToString() << "\t\t" << testReport.EquityCurve[i] << "\t\t" << testReport.FractionInvested[i] << "\n";
    }

    path = path.append("out");
    testReport.WriteToFile(path);

}

BuySellInstruction TestStrategy(DataSet* dataSet, int i, float cashOnHand, float currentlyInvested)
{
    BuySellInstruction instruction;
    instruction.BuyOrSell = buy;
    instruction.Amount = 0.1 * cashOnHand;

    return instruction;
}


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
    //report.Symbol = dataSet->Symbol;
    report.StrategyID = strategyID;

    int dataCount = dataSet->size();
    report.EquityCurve.reserve(dataCount);
    report.FractionInvested.reserve(dataCount);
    report.InvestmentDelta.reserve(dataCount);

    float totalEquity = totalInitialEquity;
    float investment = totalEquity * fractionInitiallyInvested;
    float cash = totalEquity - investment;

    for (int i = 0; i < dataCount; i++)
    {
        report.TimeStamps.push_back(dataSet->Candles[i].TimeStamp.ToString());
        report.EquityCurve.push_back(cash + investment);
        report.FractionInvested.push_back(investment / (cash + investment));

        BuySellInstruction instruction = strategy(dataSet, i, cash, investment);
        
        switch (instruction.BuyOrSell)
        {
        case buy:
            cash -= instruction.Amount;
            investment += instruction.Amount;
            report.InvestmentDelta.push_back(instruction.Amount);
            break;

        case sell:
            cash += instruction.Amount;
            investment -= instruction.Amount;
            report.InvestmentDelta.push_back(-instruction.Amount);
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

    std::cout << "Strategy executed.\n";

    return report;
}

