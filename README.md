# StockStrategyAnalysis
An API to allow testing of stock-market strategies using historical data.

## Setup Instructions
You can use the files directly as part of your project, or compile it into a .lib. If using a lib, you will only need to include the path to StockStrategyFramework.h.

## Usage Instructions
You will need to a CSV file of the data on which to test your strategy. The file must have the form:

timestamp,open,high,low,close,volume
2025-01-05,54.53,55.42,41.26,48.04,3477441
2025-01-04,55.34,59.46,51.12,54.30,3900759
...
or have timestamps of the form:
2025-01-04 10:06:40,...

The precision of the values (open/high/low/close) is arbitrary however. I wrote this to be used with the format obtained from Alpha Vantage, but as long as the format is as shown, you're fine.

Create a DataSet object using DataSetFromCSV(...). Look at your data to determine whether it is chronologically forward or backwards. A DataSet is a wrapper for a vector of Candles. A Candle is a struct with the data from one line from your CSV.

Pass the DataSet you created into ExecuteStrategy(...), together with a StrategyID (your choice) and the strategy itself. A Strategy is a function of the form; 
BuySellInstruction(*)(DataSet* dataSet, int i, float cashOnHand, float currentlyInvested);
It should create a BuySellInstruction based at the time of the i-th Candle of your DataSet, given the (not invested) cash available and the currently invested cash, in units of currency.

A BuySellInstruction is a struct containing an enum (Buy_Or_Sell, with possibilities 'buy' or 'sell') and a currency amount.

ExecuteStrategy will execute the strategy once per Candle and output result in a file.


