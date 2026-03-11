#include <fstream>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <cmath>

using namespace std;

double crossValidation(vector<vector<double>> actualSet, vector<int> list) {
    double numCorrectClassified = 0;
    vector<vector<double>> data;

    // for (int i = 1; i < list.size(); i++) {
    //     for (int j = 0; j < data.size(); ++j) {
    //         data.push_back( actualSet.at(j).at(list.at(i)) );
    //     }
            
    // }
    for (int i = 0; i < actualSet.size(); ++i) {
        vector<double> features;
        features.push_back(actualSet.at(i).at(0));
        for (int j = 0; j < list.size(); ++j) {
            features.push_back(actualSet.at(i).at(list.at(j)));
        }
        data.push_back(features);
    }

    // //modify local data vector
    // for (int i = 1; i < data.at(i).size(); ++i) {
    //     for (int j = 0; j < list.size(); ++j) {
    //         //feature is not included in the list
    //         if (list.at(j) != i) {
    //             //zero out that feature from each row
    //             for (int k = 0; k < data.size(); ++k) {
    //                 data.at(k).at(list.at(j)) = 0;
    //             }
    //         }
    //     }
    // }

    for (int i = 0; i < data.size(); ++i) {
        //store the label and features in different place
        vector<double> objectToClassify = data.at(i);
        objectToClassify.erase(objectToClassify.begin());
        double label = data.at(i).at(0);
        double nnLabel = 0;
        double nnDistance = 1000000;
        double nnLocation = 1000000;
        
        for (int j = 1; j < data.size(); ++j) {
            //neighbour is not yourself
            if (j != i) {
                //euclidean distance calculation
                //take columns of object, subtract with its corresponding columns of j and square it
                double distance = 0;
                for (int k = 0; k < objectToClassify.size(); ++k) {
                    //sum all squares for each feature
                    distance += (objectToClassify.at(k) - data.at(j).at(k+1)) * (objectToClassify.at(k) - data.at(j).at(k+1));
                }
                distance = sqrt(distance);
                //set closest neighbour
                if (distance < nnDistance) {
                    nnDistance = distance;
                    nnLocation = j;
                    nnLabel = data.at(nnLocation).at(0);
                }
            }
        }
        if (label == nnLabel) {
            ++numCorrectClassified;
        }
    }
    return numCorrectClassified / data.size();
}

double forwardSelection (vector<vector<double>> data, vector<int> &setOfFeatures, vector<int> &bestSetOfFeatures, double &bestSoFarAccuracy) {
    double bestAccuracy = 0;
    //checks i column starting from the first feature
    for (int i = 1; i < data.at(0).size(); ++i) {
        int featureToAdd;
        bool availableFeature = true;
        for (int j = 1; j < data.at(0).size(); ++j) {
            //check if the feature is already added
            for (int k = 0; k < setOfFeatures.size(); ++k) {
                availableFeature = true;
                //if feature is in the feature list, mark that this feature already exist
                if (j == setOfFeatures.at(k)) {
                    availableFeature = false;
                    break;
                } 
            }
            vector<int> list = setOfFeatures;
            list.push_back(j);
            double accuracy = crossValidation(data, list);
            if (availableFeature) {
                cout << "\t Using feature(s) {" << j << "} accuracy is " << accuracy * 100 << "%" << endl;
            }
            if (accuracy >= bestSoFarAccuracy && availableFeature) {
                bestSoFarAccuracy = accuracy;
                featureToAdd = j;
            }
        }
        setOfFeatures.push_back(featureToAdd);
        //keep track of where the accuracy goes down
        if (bestSoFarAccuracy < bestAccuracy) {
            cout << "\n(Warning, Accuracy has decreased! Continuing search in case of local maxima)"<< endl;
        }
        else {
            bestAccuracy = bestSoFarAccuracy;
            bestSetOfFeatures = setOfFeatures;
        }
        cout << "\nFeature set {";
        for (int j = 0; j < setOfFeatures.size(); ++j) {
            cout << setOfFeatures.at(j);
            if (j < setOfFeatures.size() - 1) {
                cout << ",";
            }
        }
        cout << "} was best, accuracy is " << bestSoFarAccuracy * 100 << "%" << endl << endl;
        bestSoFarAccuracy = 0;
    }
    return bestAccuracy;
}

double backwardElimination (vector<vector<double>> data, vector<int> &setOfFeatures, vector<int> &bestSetOfFeatures, double &bestSoFarAccuracy) {
    double bestAccuracy = 0;
    //add all features
    for (int i = 1; i < data.at(0).size(); ++i) {
        setOfFeatures.push_back(i);
    }
    //checks i column starting from the first feature
    for (int i = 1; i < data.at(0).size(); ++i) {
        int featureToDelete;

        //j = the number of the feature
        for (int j = 0; j < setOfFeatures.size(); ++j) {
            vector<int> list = setOfFeatures;
            for (int k = 0; k < list.size(); ++k) {
                if (k == j) {
                    list.erase(list.begin() + k);
                    break;
                }
            }
            double accuracy;
            if (list.size() != 0) {
                accuracy = crossValidation(data, list);
            }


            cout << "\t Taking out feature(s) {" << setOfFeatures.at(j) << "} accuracy is " << accuracy * 100 << "%" << endl;
            
            if (accuracy >= bestSoFarAccuracy) {
                bestSoFarAccuracy = accuracy;
                featureToDelete = setOfFeatures.at(j);
            }
        }
        //delete feature from the list
        for (int j = 0; j < setOfFeatures.size(); ++j) {
            if (setOfFeatures.at(j) == featureToDelete) {
                setOfFeatures.erase(setOfFeatures.begin() + j);
                break;
            }

        }
        //keep track of where the accuracy goes down
        if (bestSoFarAccuracy < bestAccuracy) {
            cout << "\n(Warning, Accuracy has decreased! Continuing search in case of local maxima)" << endl;
        }
        else {
            bestAccuracy = bestSoFarAccuracy;
            bestSetOfFeatures = setOfFeatures;
        }

        cout << "\nFeature set {";
        for (int j = 0; j < setOfFeatures.size(); ++j) {
            cout << setOfFeatures.at(j);
            if (j < setOfFeatures.size() - 1) {
                cout << ",";
            }
        }
        cout << "} was best, accuracy is " << bestSoFarAccuracy * 100 << "%" << endl << endl;
        bestSoFarAccuracy = 0;
    }
    return bestAccuracy;
}

void search(vector<vector<double>> data) {
    vector<int> setOfFeatures;
    vector<int> bestSetOfFeatures;
    double bestAccuracy = 0;
    double accuracy = 0;
    vector<int> finalBestSetOfFeatures;
    
    cout << "Beginning Search." << endl << endl;
    double bestSetAccuracyForward = forwardSelection(data, setOfFeatures, bestSetOfFeatures, accuracy);
    finalBestSetOfFeatures = bestSetOfFeatures;
    setOfFeatures.clear();
    bestSetOfFeatures.clear();
    
    double bestSetAccuracyBackward = backwardElimination(data, setOfFeatures, bestSetOfFeatures, accuracy);
    //set the best accuracy 
    if (bestSetAccuracyForward > bestSetAccuracyBackward) {
        bestAccuracy = bestSetAccuracyForward;
    }
    else {
        bestAccuracy = bestSetAccuracyBackward;
        finalBestSetOfFeatures = bestSetOfFeatures;
    }

    cout << "Finished search!! The best feature subset is {";
    for (int i = 0; i < finalBestSetOfFeatures.size(); ++i) {
        cout << finalBestSetOfFeatures.at(i);
        if (i < finalBestSetOfFeatures.size() - 1) {
            cout << ",";
        }
    }
    cout << "}, which has an accuracy of " << bestAccuracy * 100 << "%" << endl;
}

int main() {
    ifstream dataFile;
    vector<vector<double>> data;
    int i = 0;
    string line;
    string dataPt;

    dataFile.open("CS170_Small_DataSet__86.csv");
    //dataFile.open("SanityCheck_DataSet__1.csv");
    //dataFile.open("SanityCheckDataSet__2.csv");
    //dataFile.open("CS170_Large_DataSet__33.csv");

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