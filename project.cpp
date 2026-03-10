#include <fstream>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

int crossValidation(vector<vector<double>> data, vector<int> list, int feature) {
    int accuracy = rand();
}

int main() {
    ifstream dataFile;
    vector<vector<double>> data;
    int i = 0;
    string line;
    string dataPt;

    dataFile.open("CS170_Small_DataSet__86.csv");

    if (!dataFile.is_open()) {
        return -1;
    }

    // organize the txt file into a 2 dimensional vector
    while (getline(dataFile, line)) {

        //split tokens per row https://stackoverflow.com/questions/10058606/splitting-a-string-by-a-character
        stringstream ss(line);
        double value;
        vector<double> rows;
        // https://stackoverflow.com/questions/44999976/how-to-extract-double-from-a-stringstream
        while (ss >> value) {  
            rows.push_back(value);
        }

        data.push_back(rows);
    }

    search(data);
    
    return 0;
};