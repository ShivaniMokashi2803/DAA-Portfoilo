#include <iostream>
#include <fstream>
#include <string>
#include <conio.h> // For getch() and hiding password on Windows
#include <cctype>  // For checking character types
using namespace std;

// ANSI color codes for better visuals
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define MAGENTA "\033[35m"

// Function prototypes for user profile management
void signUp();
void login();
void logout();
void updateProfile(const string &username);
bool isEmailExists(const string &email);
string hidePassword();
bool isValidPassword(const string &password);

// Display a header with the given title
void displayHeader(const string &title) {
    system("cls"); // Clears the screen (Windows-specific, use clear for Linux/Mac)
    cout << MAGENTA << "==========================================" << RESET << endl;
    cout << GREEN << "          " << title << RESET << endl;
    cout << MAGENTA << "==========================================" << RESET << endl;
}

// Sign-up function
void signUp() {
    string username, name, email, classInfo, password;

    displayHeader("Sign Up Page");
    cout << YELLOW << "Please enter your details to sign up:" << RESET << endl;
    cout << "Username: ";
    cin.ignore();
    getline(cin, username);

    cout << "Name: ";
    getline(cin, name);

    cout << "Email: ";
    getline(cin, email);

    if (isEmailExists(email)) {
        cout << RED << "Email already exists! Please try logging in or use a different email." << RESET << endl;
        return;
    }

    cout << "Class: ";
    getline(cin, classInfo);

    cout << "Password: ";
    password = hidePassword();

    while (!isValidPassword(password)) {
        cout << RED << "\nPassword must be 8 characters long and include digits, letters, and special characters." << RESET << endl;
        cout << "Please enter a valid password: ";
        password = hidePassword();
    }

    ofstream file("user_profile.txt", ios::app);
    if (file.is_open()) {
        file << username << "," << name << "," << email << "," << classInfo << "," << password << endl;
        file.close();
        cout << GREEN << "\nSign up successful! Redirecting to main menu..." << RESET << endl;
    } else {
        cout << RED << "Error saving user data!" << RESET << endl;
    }

    cout << YELLOW << "Press any key to return to the main menu..." << RESET;
    _getch();
}

// Login function
void login() {
    string inputEmail, inputPassword;

    displayHeader("Login Page");
    cout << YELLOW << "Please enter your login credentials:" << RESET << endl;
    cout << "Email: ";
    cin.ignore();
    getline(cin, inputEmail);

    cout << "Password: ";
    inputPassword = hidePassword();

    ifstream file("user_profile.txt");
    if (!file.is_open()) {
        cout << RED << "Error reading user data!" << RESET << endl;
        return;
    }

    string line, username, name, email, classInfo, password;
    bool isLoggedIn = false;
    while (getline(file, line)) {
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        size_t pos3 = line.find(',', pos2 + 1);
        size_t pos4 = line.find(',', pos3 + 1);

        username = line.substr(0, pos1);
        name = line.substr(pos1 + 1, pos2 - pos1 - 1);
        email = line.substr(pos2 + 1, pos3 - pos2 - 1);
        classInfo = line.substr(pos3 + 1, pos4 - pos3 - 1);
        password = line.substr(pos4 + 1);

        if (email == inputEmail && password == inputPassword) {
            displayHeader("Welcome!");
            cout << GREEN << "Hello, " << name << "! You have successfully logged in." << RESET << endl;
            cout << "Class: " << classInfo << endl;
            isLoggedIn = true;
            break;
        }
    }
    file.close();

    if (!isLoggedIn) {
        cout << RED << "\nInvalid email or password. Please try again." << RESET << endl;
    }

    cout << YELLOW << "Press any key to return to the main menu..." << RESET;
    _getch();
}

// Utility Functions
bool isEmailExists(const string &email) {
    ifstream file("user_profile.txt");
    string line;
    while (getline(file, line)) {
        size_t pos2 = line.find(',');
        size_t pos3 = line.find(',', pos2 + 1);
        string existingEmail = line.substr(pos2 + 1, pos3 - pos2 - 1);
        if (existingEmail == email) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

string hidePassword() {
    string password;
    char ch;
    while ((ch = _getch()) != '\r') { // '\r' is the Enter key
        if (ch == '\b') { // Backspace
            if (!password.empty()) {
                password.pop_back();
                cout << "\b \b"; // Erase the last * on screen
            }
        } else {
            password.push_back(ch);
            cout << "*";
        }
    }
    cout << endl;
    return password;
}

bool isValidPassword(const string &password) {
    if (password.length() != 8) return false; // Check length
    bool hasLetter = false, hasDigit = false, hasSpecialChar = false;

    for (char ch : password) {
        if (isalpha(ch)) hasLetter = true;
        else if (isdigit(ch)) hasDigit = true;
        else if (ispunct(ch)) hasSpecialChar = true;
    }

    return hasLetter && hasDigit && hasSpecialChar; // All conditions must be true
}
