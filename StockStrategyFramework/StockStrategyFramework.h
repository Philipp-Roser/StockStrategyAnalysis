#pragma once

#include <string>

#include "DataStructures.h"


using Strategy = BuySellInstruction(*)(DataSet* dataSet, int i, float cashOnHand, float currentlyInvested);

/// <summary>
/// Create a DataSet (essentially a wrapper for a list of Candles) from a csv file. The file should be formatted
/// with a header line "timestamp,open,high,low,close,volume", followed by lines of data in that order. 
/// E.g. csv files obtained from Alpha Vantage (see my utility for that) have that format.
/// </summary>
DataSet DataSetFromCSV(std::string csvPath, bool isInReverseChronologicalOrder = true);


/// <summary>
/// Executes the strategy throughout the specified DataSet, where a Strategy is a function of the form
/// <para>BuySellInstruction MyStrategy(DataSet* dataSet, int i, float cashOnHand, float currentlyInvested) </para>
/// where i is the index of the data point (i.e. i-th candle), and cashOnHand and currentlyInvested are in 
/// units of currency (e.g. dollars).
/// </summary>
StrategyReport ExecuteStrategy(
    std::string strategyID,
    Strategy strategy,
    DataSet* dataSet,
    float totalInitialEquity = 1000,
    float fractionInitiallyInvested = 0.5);