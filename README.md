# StockStrategyAnalysis
An API to allow testing of stock-market strategies using historical data.

## Setup Instructions
You can use the files directly as part of your project, or compile it into a .lib. If using a lib, you will only need to include the path to StockStrategyFramework.h.

## Usage Instructions
You will need to a CSV file of the data on which to test your strategy. The file must have the form:
```
timestamp,open,high,low,close,volume
2025-01-05,54.53,55.42,41.26,48.04,3477441
2025-01-04,55.34,59.46,51.12,54.30,3900759
...
```
or have timestamps of the form:
```
2025-01-04 10:06:40,...
```

The precision of the values (open/high/low/close) is arbitrary however. I wrote this to be used with the format obtained from Alpha Vantage, but as long as the format is as shown, you're fine.

Create a DataSet object using DataSetFromCSV(...). Look at your data to determine whether it is chronologically forward or backwards. A DataSet is a wrapper for a vector of Candles. A Candle is a struct with the data from one line from your CSV.

Pass the DataSet you created into ExecuteStrategy(...), together with a StrategyID (your choice) and the strategy itself. A Strategy is a function of the form:
```
BuySellInstruction(*)(DataSet* dataSet, int i, float cashOnHand, float currentlyInvested);
```
It should create a BuySellInstruction based at the time of the i-th Candle of your DataSet, given the (not invested) cash available and the currently invested cash, in units of currency.

A BuySellInstruction is a struct containing an enum Buy_Or_Sell ('buy' or 'sell') and a currency amount.

ExecuteStrategy(...) will execute the strategy once per Candle and return a StrategyReport struct. Inspect the members of your StrategyReport directly and process them as you deem helpful, or use StrategyReport::WriteToFile(..) to obtain an output file with the detailed result of your strategy.

Here is an example of a (probably not very good) strategy:
```
BuySellInstruction MyBrilliantStrategy(DataSet* dataSet, int i, float cashOnHand, float currentlyInvested)
{
  // If today's open is higher than yesterday's open, spend half my cash to buy more shares
  // Otherwise, sell half.

  BuySetllInstruction instruction;

  if(i == 0)
    return instruction; // do nothing on the first day

  if( (*dataSet)[i].open > (*dataSet)[i-1].open)
  {
    instruction.BuyOrSell = buy;
    instruciton.Amount = 0.5 * cashOnHand;
  }
  else
  {
    instruction.BuyOrSell = sell;
    instruction.Amount = 0.5 * currentlyInvested;  
  }

  return instruction;
}
```

In order to process multiple strategies in bulk (e.g. similar strategies with differently tuned parameters), use
```
StrategiesMetaReport ExecuteStrategyCollection(
    std::vector<std::string> strategyIDs, 
    std::vector<Strategy> strategies,
    DataSet* dataSet,
    float totalInitialEquity = 1000,
    float totalInitiallyInvested = 0.5);
```
The StrategiesMetaReport contains all the individual strategy reports, as well as information about the best-performing one. Its declaration is
```
struct StrategiesMetaReport
{
    std::vector<StrategyReport> StategyReports;

    float LargestGrowth;
    StrategyReport BestStrategy;

    void WriteBestToFile(std::string path);
};
```


