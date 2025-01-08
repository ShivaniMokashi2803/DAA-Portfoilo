#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// Function prototypes for assessment and feedback
void startAssessment(const string &username);
void giveFeedback(const string &username);

// Function to handle assessments
void startAssessment(const string &username) {
    string answer;
    int score = 0;

    cout << CYAN << "\nStarting Assessment for " << username << RESET << endl;
    cout << "Q1: What is 2 + 2? \n[A] 3 \n[B] 4 \n[C] 5 \nYour Answer: ";
    cin >> answer;

    if (answer == "B" || answer == "b") score++;

    // Additional questions can be added here

    cout << GREEN << "\nAssessment Completed! Your score: " << score << RESET << endl;
}

// Function to collect feedback
void giveFeedback(const string &username) {
    string feedback;

    cout << CYAN << "\nWe value your feedback, " << username << RESET << endl;
    cout << "Enter your feedback: ";
    cin.ignore();
    getline(cin, feedback);

    ofstream file("feedback.txt", ios::app);
    if (file.is_open()) {
        file << username << ": " << feedback << endl;
        file.close();
        cout << GREEN << "Thank you for your feedback!" << RESET << endl;
    } else {
        cout << RED << "Error saving feedback!" << RESET << endl;
    }
}
