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

void search(vector<vector<double>> data) {
    vector<int> setOfFeatures;

    //checks i column starting from the first feature
    for (int i = 1; i < data.at(i).size(); ++i) {
        cout << "On the " << i << "th level of the search tree" << endl;
        int featureToAdd;
        int bestSoFarAccuracy = 0;
        bool availableFeature = true;

        for (int j = 1; j < data.at(j).size(); ++j) {
            //check if the feature is already added
            for (int k = 0; k < setOfFeatures.size(); ++k) {
                availableFeature = true;
                //if feature is in the feature list, mark that this feature already exist
                if (j == setOfFeatures.at(k)) {
                    availableFeature = false;
                    //cout << j << " " << setOfFeatures.at(k) << " " << featureToAdd << endl;
                    break;
                } 
            }

            //the feature we're checking right now isn't in the list
            if (availableFeature) {
                cout << "--Considering adding the " << j << " feature" << endl;
            }
            
            int accuracy = crossValidation(data, setOfFeatures, j+1);

            if (accuracy > bestSoFarAccuracy && availableFeature) {
                bestSoFarAccuracy = accuracy;
                featureToAdd = j;
            }
        }
 
        setOfFeatures.push_back(featureToAdd);
        cout << "On level " << i << " I added feature " << featureToAdd << " to the current set" << endl;
    }
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