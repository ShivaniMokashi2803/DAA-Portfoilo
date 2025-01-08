#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// Function prototype for progress tracking
void viewPerformance(const string &username);

// Function to view performance
void viewPerformance(const string &username) {
    ifstream file("performance.txt");
    if (!file.is_open()) {
        cout << RED << "Error opening performance file!" << RESET << endl;
        return;
    }

    string line;
    bool recordFound = false;
    cout << CYAN << "\nPerformance Report for " << username << RESET << endl;
    while (getline(file, line)) {
        if (line.find(username) != string::npos) {
            cout << line << endl;
            recordFound = true;
        }
    }
    file.close();

    if (!recordFound) {
        cout << RED << "No performance data found for " << username << "!" << RESET << endl;
    }
}
