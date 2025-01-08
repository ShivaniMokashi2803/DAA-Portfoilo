#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// ANSI color codes for better visuals
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define CYAN "\033[36m"
#define YELLOW "\033[33m"

// Function prototype for managing content
void manageContent(const string &classInput);

void manageContent(const string &classInput) {
    ifstream file("content.txt");
    if (!file.is_open()) {
        cout << RED << "Failed to open content file!" << RESET << endl;
        return;
    }

    string line, currentSubject = "", currentClass = "";
    vector<string> chapters;
    vector<string> websites;
    vector<string> videos;
    vector<string> oneShorts;

    bool classFound = false;
    bool subjectFound = false;

    while (getline(file, line)) {
        if (line.find("Class:") != string::npos) {
            currentClass = line.substr(6); // Extract class number after "Class:"
            if (currentClass == classInput) {
                classFound = true;
                subjectFound = false; // Reset subject flag
            } else {
                classFound = false;
            }
        }

        if (classFound && line.find("Subject:") != string::npos) {
            currentSubject = line.substr(8); // Extract subject after "Subject:"
            cout << CYAN << "\nSubject: " << currentSubject << RESET << endl;
            subjectFound = true;
            chapters.clear();
            websites.clear();
            videos.clear();
            oneShorts.clear();
        }

        if (classFound && subjectFound && line.find("Chapter") != string::npos) {
            chapters.push_back(line); // Add chapter name

            // Extract subsequent resources
            if (getline(file, line)) websites.push_back(line.substr(9)); // Website
            if (getline(file, line)) videos.push_back(line.substr(7));   // Video
            if (getline(file, line)) oneShorts.push_back(line.substr(9)); // One Short
        }
    }

    if (!classFound) {
        cout << RED << "Class not found in the file!" << RESET << endl;
        return;
    }

    if (chapters.empty()) {
        cout << RED << "No chapters found for the selected class and subject!" << RESET << endl;
        return;
    }

    // Display Chapter Choices
    int chapterChoice;
    cout << CYAN << "\nSelect Chapter:\n" << RESET;
    for (size_t i = 0; i < chapters.size(); i++) {
        cout << "[" << i + 1 << "] " << chapters[i] << endl;
    }

    cout << YELLOW << "\nEnter your choice: " << RESET;
    cin >> chapterChoice;

    if (chapterChoice < 1 || chapterChoice > chapters.size()) {
        cout << RED << "Invalid chapter choice!" << RESET << endl;
        return;
    }

    // Display Resource Choices
    int resourceChoice;
    cout << CYAN << "\nSelect Resource:\n" << RESET;
    cout << "[1] Website\n";
    cout << "[2] Video\n";
    cout << "[3] One Short Video\n";

    cout << YELLOW << "\nEnter your choice: " << RESET;
    cin >> resourceChoice;

    switch (resourceChoice) {
        case 1:
            cout << GREEN << "\nWebsite Link: " << websites[chapterChoice - 1] << RESET << endl;
            break;
        case 2:
            cout << GREEN << "\nVideo Link: " << videos[chapterChoice - 1] << RESET << endl;
            break;
        case 3:
            cout << GREEN << "\nOne Short Video Link: " << oneShorts[chapterChoice - 1] << RESET << endl;
            break;
        default:
            cout << RED << "Invalid resource choice!" << RESET << endl;
    }

    file.close();
}
