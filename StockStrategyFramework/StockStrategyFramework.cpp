#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include <windows.h> // TEMP


#include "DataStructures.h"

typedef std::string string;






// forward declarations:
std::vector<Candle> DataSetFromCSV(string csvPath);


int main()
{
    std::cout << "Test\n";

    char buffer[MAX_PATH];
    DWORD length = GetModuleFileNameA(NULL, buffer, MAX_PATH);
    string path = std::string(buffer, length);
    size_t lastSlash = path.find_last_of("\\/");
    path = path.substr(0, lastSlash);
    path.append("/VTI_Daily.csv");
    std::cout << "Filepath: " << path << "\n";

    DataSetFromCSV(path);
}


std::vector<Candle> DataSetFromCSV(string csvPath)
{
    std::ifstream file(csvPath);
    std::vector<Candle> dataSet;
    
    string headerLine;
    std::getline(file, headerLine);
    if (headerLine != "timestamp,open,high,low,close,volume")
    {
        std::cout << "First line is:\n" << headerLine << "\n but should read: \"timestamp,open,high,low,close,volume\".";
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

        dataSet.push_back(Candle(timeStamp, open, high, low, close, volume));
    }

    std::cout << "Created data set with " << dataSet.size() << " candles.\n";
    std::cout << "Date starts at " << dataSet[0].TimeStamp.ToString();
    return dataSet;
    

}

