#include <iostream>
#include <iomanip>
#include "menu.h" 
#include "db.h"// Include the menu header file
#include <conio.h>

using namespace std;

// ANSI color codes as const strings
const string RESET = "\033[0m";      // Reset to default color
const string RED = "\033[91m";       // Red text
const string YELLOW = "\033[93m";    // Yellow text
const string CYAN = "\033[96m";      // Blue text (light cyan)
const string GREEN = "\033[32m";     // Green text
const string WHITE = "\033[37m";     // White text
const string BWHITE = "\033[1;37m";  // BOLD WHITE
const string BYELLOW = "\033[1;93m"; // BOLD YELLOW
const string BCYAN = "\033[1;96m"; //BOLD CYAN

int main() {
    int choice;
    Menu menu;
    while (true) {
        menu.displayMainMenu();
        cin >> choice;
        if (cin.fail() || cin.peek() != '\n') {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            system("cls");
            cout << RED << "Invalid input! Please try again!" << RESET << endl;
            continue;
        }

        // Clear screen on Windows (system("cls")) and process choice
        if (choice == 1) {
            system("cls");
            cout << CYAN << "=== Login ===" << RESET; // Reset color to default
            string inputUsername, inputPassword;
            cout << endl;
            cout << YELLOW << "Enter Username: " << RESET;
            cin >> inputUsername;
            cout << YELLOW << "Enter Password: " << RESET;
            char ch;
            while ((ch = _getch()) != '\r') { // '\r' is the Enter key in Windows
                if (ch == '\b') { // Handle Backspace
                    if (!inputPassword.empty()) {
                        inputPassword.pop_back();
                        cout << "\b \b"; // Remove last '*'
                    }
                }
                else {
                    inputPassword += ch;
                    cout << "*";
                }
            }
            cout << endl;
            bool loginSuccess = menu.login(inputUsername, inputPassword);  // Assuming login function returns true on successful login
            cout << RESET; // Reset color to default
            cout << endl;

            // If login is successful, navigate to user menu
            if (loginSuccess) {
                DBConnection db;
                db.prepareStatement("SELECT * FROM user WHERE BINARY Username = ? AND BINARY Password = ?");
                db.stmt->setString(1, inputUsername);
                db.stmt->setString(2, inputPassword);
                db.QueryResult();
                //db.res->next();
                if (!db.res->next()) {
                    db.prepareStatement("SELECT * FROM administrator WHERE BINARY Username = ? AND BINARY Password = ?");
                    db.stmt->setString(1, inputUsername);
                    db.stmt->setString(2, inputPassword);
                    db.QueryResult();
                    db.res->next();
                    menu.adminMenu();
                }
                else {
                    int userID = db.res->getInt("User_ID");
                    menu.userMenu(userID);
                }// Navigate to user menu
            }
        }

        else if (choice == 2) {
            system("cls"); 
            cout << WHITE;
            cout << CYAN << "=== Register ===" << RESET << endl;
            menu.registerUser();
            cout << RESET; // Reset color to default
            cout << endl;
        }

        else if (choice == 3) {
            system("cls");
            cout << CYAN << "=== Forgot Password ===" << RESET << endl;
            cin.ignore();
            string username, email;
            cout << BYELLOW << "Enter Username: " << RESET;
            getline(cin, username);

            // Ensure no empty username input
            while (username.empty()) {
                cout << RED << "Username cannot be empty. Please enter a valid username: " << RESET;
                getline(cin, username);
            }

            cout << BYELLOW << "Enter Email: " << RESET;
            getline(cin, email);

            // Ensure no empty email input
            while (email.empty()) {
                cout << RED << "Email cannot be empty. Please enter a valid email: " << RESET;
                getline(cin, email);
            }

            try {
                // Call the forgotPassword method and pass user input
                menu.forgotPassword(username, email);
            }
            catch (const std::exception& e) {
                // Catch any unexpected errors
                cout << RED << "An error occurred: " << e.what() << RESET << endl;
            }

            cout << RESET; // Reset color to default
            cout << endl;
        }

        else if (choice == 0) {
            system("cls");
            cout << YELLOW;
            cout << "Thank you for using the platform. Goodbye!" << endl;
            cout << RESET;
            return 0; // Exit the program
        }

        else {
            system("cls");
            cout << RED; // Red for invalid choice
            cout << "Invalid choice. Please select 1, 2, or 3." << endl;
            cout << RESET; // Reset color to default
        }
    }
    return 0;
}