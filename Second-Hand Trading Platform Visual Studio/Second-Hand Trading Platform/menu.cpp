#include <iostream>
#include <iomanip>
#include <mysql/jdbc.h>
#include "db.h"
#include "item.h"
#include "menu.h" // Include the corresponding header file
#include <conio.h>
#include <string>

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

Item item;
// Display Main Menu 
void Menu::displayMainMenu() {
    // Display title with color
    cout << GREEN << "===========================================================" << RESET << endl;  // Green text for title
    cout << CYAN  << "                   Second-Hand Platform                    " << RESET << endl;
    cout << GREEN << "===========================================================" << RESET << endl;

    // Display menu options
    cout << BWHITE << endl << "1. Login" << RESET << endl;  // White text for menu options
    cout << BWHITE << "2. Register" << RESET << endl;
    cout << BWHITE << "3. Forgot Password" << RESET << endl;
    cout << BWHITE << "0. Exit" << RESET << endl << endl;
     
    // User input
    cout << BYELLOW << "Enter your choice: " << RESET;  // Yellow text for prompt
}

// Login function
bool Menu::login(string username, string password) {

    DBConnection db;
    db.prepareStatement("SELECT * FROM user WHERE BINARY Username = ? AND BINARY Password = ?");
    db.stmt->setString(1, username);
    db.stmt->setString(2, password);
    db.QueryResult();

    if (db.res->next()) {
        system("cls");
        cout << YELLOW << "Login successful. Welcome, " << RESET
            << CYAN << db.res->getString("Name") << RESET
            << YELLOW << "! You are logged in as a normal user." << RESET << endl;
        return true;
    }
    else {
        db.prepareStatement("SELECT * FROM administrator WHERE BINARY Username = ? AND BINARY Password = ?");
        db.stmt->setString(1, username);
        db.stmt->setString(2, password);
        db.QueryResult();

        if (db.res->next()) {
            system("cls");
            cout << YELLOW  << "Login successful! You are logged in as an administrator." << RESET << endl;
            return true;
        }
        else {
            system("cls");
            cout << RED << "Login failed. Invalid username or password." << RESET << endl;
            return false;
        }
    }
}

// Register function
void Menu::registerUser() {
    string username, password, email, phone, name, address;
    
    cin.ignore();

    cout << YELLOW << "Enter Username: " << RESET;
    getline(cin, username);
    if (username.empty()) {
        cout << RED << "Error: Username cannot be empty!" << RESET << endl;
        return;
    }
    else if (username.find(' ') != string::npos) {
        cout << RED << "Error: Username cannot contain spaces!" << RESET <<endl;
        return;
    }

    cout << YELLOW << "Enter Password: " << RESET;
    getline(cin, password);
    if (password.empty()) {
        cout << RED << "Error: Password cannot be empty!" << RESET << endl;
        return;
    }
    else if (password.find(' ') != string::npos) {
        cout << RED << "Error: Password cannot contain spaces!" << RESET << endl;
        return;
    }

    cout << YELLOW << "Enter Email: " << RESET;
    getline(cin, email);
    if (email.empty()) {
        cout << RED << "Error: Email cannot be empty!" << RESET << endl;
        return;
    }
    else if (email.find('@') == string::npos) {
        cout << RED << "Error: Email must contain '@'!" << RESET << endl;
        return;
    }
    else if (email.find('.com') == string::npos) {
        cout << RED << "Error: Email must contain '.com'!" << RESET << endl;
        return;
    }
    else if (email.find(' ') != string::npos) {
        cout << RED << "Error: Email cannot contain spaces!" << RESET << endl;
        return;
    }

    cout << YELLOW << "Enter Phone Number: " << RESET;
    getline(cin, phone);
    if (phone.empty()) {
        cout << RED << "Error: Phone Number cannot be empty!" << RESET << endl;
        return;
    }
    else if (phone.find(' ') != string::npos) {
        cout << RED << "Error: Phone Number cannot contain spaces!" << RESET << endl;
        return;
    }
    else if (phone.length() < 10 || phone.length() > 11) {
        cout << RED << "Error: Phone Number is invalid!" << RESET << endl;
        return;
    }
    // Check if the phone number contains only digits
    else if (phone.find_first_not_of("0123456789") != string::npos) {
        cout << RED << "Error: Phone Number must contain only digits!" << RESET << endl;
        return;
    }

    cout << YELLOW << "Enter Real Name: " << RESET;
    getline(cin, name);
    if (name.empty()) {
        cout << RED <<"Error: Name cannot be empty!" << RESET << endl;
        return;
    }
    else if (name.find_first_of("0123456789") != string::npos) {
        cout << RED << "Error: Name cannot contain numbers!" << RESET << endl;
        return;
    }
    else if (name.find_first_of("!@#$%^&*()_+=<>?\\|{}[]:;\"'`~,") != string::npos) {
        cout << RED << "Error: Name cannot contain symbols!" << RESET << endl;
        return;
    }

	cout << YELLOW << "Enter Address: " << RESET;
	getline(cin, address);
    if (address.empty()) {
        cout << RED << "Error: Address cannot be empty!" << RESET << endl;
        return;
    }    

    DBConnection db;

    // Check for duplicate username
    db.prepareStatement("SELECT COUNT(*) FROM user WHERE Username = ?");
    db.stmt->setString(1, username);
    db.QueryResult();
    if (db.res->next() && db.res->getInt(1) > 0) {
        cout << RED << "Error: Username already exists!" << RESET << endl;
        return;
    }

    // Check for duplicate email
    db.prepareStatement("SELECT COUNT(*) FROM user WHERE Email = ?");
    db.stmt->setString(1, email);
    db.QueryResult();
    if (db.res->next() && db.res->getInt(1) > 0) {
        cout << RED << "Error: Email already exists!" << RESET << endl;
        return;
    }

    // Check for duplicate phone number
    db.prepareStatement("SELECT COUNT(*) FROM user WHERE Phone = ?");
    db.stmt->setString(1, phone);
    db.QueryResult();
    if (db.res->next() && db.res->getInt(1) > 0) {
        cout << RED << "Error: Phone Number already exists!" << RESET << endl;
        return;
    }

    // Step 1: Get the highest User_ID from the database
    db.prepareStatement("SELECT MAX(User_ID) FROM user");
    db.QueryResult();

    int newUserID = 10001;  // Start User_ID from 10001

    // If there are existing users, we get the max User_ID and increment it
    if (db.res->next()) {
        int maxUserID = db.res->getInt(1);
        if (maxUserID > 0) {
            newUserID = maxUserID + 1;  // Increment the highest User_ID by 1
        }
    }

    db.prepareStatement("INSERT INTO user (User_ID, Username, Password, Email, Phone, Name, Address) VALUES (?, ?, ?, ?, ?, ?, ?)");
    db.stmt->setInt(1, newUserID);  // Manually set User_ID
    db.stmt->setString(2, username);
    db.stmt->setString(3, password);
    db.stmt->setString(4, email);
    db.stmt->setString(5, phone);
    db.stmt->setString(6, name);
    db.stmt->setString(7, address);

    db.stmt->executeUpdate();
    cout << endl << YELLOW << "Registration successful. Welcome, " << RESET << CYAN << name << RESET << YELLOW << "!" << RESET << endl;
    cout << YELLOW << "Press any key to continue..." << RESET;
    _getch();  // Wait for a key press
    system("cls");
}

//Forget Password
void Menu::forgotPassword(string username, string email) {
    DBConnection db;

    try {
        // Step 1: Validate username and email from the database
        db.prepareStatement("SELECT User_ID FROM user WHERE Username = ? AND Email = ?");
        db.stmt->setString(1, username);
        db.stmt->setString(2, email);
        db.QueryResult();

        if (db.res->next()) {
            int userID = db.res->getInt("User_ID");

            // Step 2: Prompt user for a new password
            string newPassword, confirmPassword;

            cout << BCYAN << endl << "Username and Email matched. You can now reset your password." << RESET << endl;

            while (true) {
                cout << BYELLOW << "Enter your new password: " << RESET;
                cin >> newPassword;

                cout << BYELLOW << "Confirm your new password: "<< RESET;
                cin >> confirmPassword;

                if (newPassword == confirmPassword) {
                    // Step 3: Update the password in the database
                    db.prepareStatement("UPDATE user SET Password = ? WHERE User_ID = ?");
                    db.stmt->setString(1, newPassword); // Use proper password hashing in production
                    db.stmt->setInt(2, userID);
                    db.stmt->executeUpdate();

                    cout << BCYAN << endl << "Password reset successfully!" << RESET << endl;
                    cout << YELLOW << endl << "Press any key to continue..." << RESET;
                    _getch();  // Wait for a key press
                    system("cls");
                    break;
                }
                else {
                    cout << RED << "Passwords do not match." << RESET << endl;
                }
            }
        }
        else {
            // Step 4: Handle case where username and email don't match
            cout << RED << endl << "No matching account found for the provided username and email." << RESET << endl;
            cout << YELLOW << endl << "Press any key to continue..." << RESET;
            _getch();  // Wait for a key press
            system("cls");
        }
    }
    catch (sql::SQLException& e) {
        cout << "Error: " << e.what() << endl;
    }
}

//User Menu
void Menu::userMenu(int userid) {
    DBConnection db;
    int userChoice;
userMenu:
    while (true) {
        // Display user menu options
        cout << GREEN << "========================================" << RESET << endl;
        cout << CYAN << "               USER MENU               " << RESET << endl;
        cout << GREEN << "========================================" << RESET << endl << endl;
        cout << BWHITE << "1. Manage Your Item" << RESET << endl;
        cout << BWHITE << "2. Buy Items" << RESET << endl;
        cout << BWHITE << "3. View & Update Order Status" << RESET << endl;
        cout << BWHITE << "4. My Purchase" << RESET << endl;
        cout << BWHITE << "5. My Sales" << RESET << endl;
        cout << BWHITE << "6. My Profile" << RESET << endl;
        cout << BWHITE << "0. Logout" << RESET << endl;
        cout << YELLOW << endl << "Enter your choice: " << RESET;
        cin >> userChoice;

        if (cin.fail() || cin.peek() != '\n') {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            system("cls");
            cout << RED << "Invalid input! Please enter a numeric choice." << RESET << endl;
            continue;
        }

        //Manage Item Menu
        if (userChoice == 1) {
            system("cls");
            while (true) {
                int manageItemChoice;
                cout << GREEN << "========================================" << RESET << endl;
                cout << CYAN << "               MANAGE ITEM               " << RESET << endl;
                cout << GREEN << "========================================" << RESET << endl << endl;
                cout << BWHITE << "1. Add Your Item" << RESET << endl;
                cout << BWHITE << "2. Delete Your Item" << RESET << endl;
                cout << BWHITE << "3. Edit Your Item" << RESET << endl;
                cout << BWHITE << "4. Display Your Available Item" << RESET << endl;
                cout << BWHITE << "0. Return to User Menu" << RESET << endl;
                cout << YELLOW << endl << "Enter your choice: " << RESET;
                cin >> manageItemChoice;
                if (cin.fail() || cin.peek() != '\n') {
                    cin.clear(); // Clear the error flag
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                    system("cls");
                    cout << RED << "Invalid input! Please enter a numeric choice." << RESET << endl;
                    continue;
                }
                //Add Item
                if (manageItemChoice == 1) {
                    system("cls");
                    cout << GREEN << "========================================" << RESET << endl;
                    cout << CYAN << "               ADD ITEM                " << RESET << endl;
                    cout << GREEN << "========================================" << RESET << endl;

                    string name, description;
                    int category;
                    double price;
                    string priceInput;

                    while (true) {
                        //Item name
                        cout << YELLOW << "Enter Item name:" << RESET;
                        cin.ignore();
                        getline(cin, name);
                        if (name.empty()) {
                            cout << RED << "Item Name cannot be empty. Please try again." << RESET << endl;
                            continue;
                        }

                        //Category
                        cout << YELLOW << "Select Category:" << RESET << endl;
                        cout << CYAN << "   1. Book" << endl;
                        cout << "   2. Furniture" << endl;
                        cout << "   3. Electronic" << endl;
                        cout << "   4. Sport" << endl;
                        cout << "   5. Others" << RESET << endl;
                        while (true) {
                            cout << YELLOW << "Enter your choice (1-5): " << RESET;
                            cin >> category;

                            // Check if input is valid
                            if (cin.fail() || cin.peek() != '\n') {
                                cin.clear(); // Clear the error flag
                                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                                cout << RED << "Invalid input! Please enter a number between 1 and 5." << RESET << endl;
                                continue;
                            }

                            // Check if category is within the valid range
                            else if (category >= 1 && category <= 5) {
                                break; // Valid input, exit the loop
                            }
                            else {
                                cout << RED << "Invalid input! Please enter a number between 1 and 5." << RESET << endl;
                                continue;
                            }
                        }
                        switch (category) {
                        case 1:
                            category = 801; // Book
                            break;
                        case 2:
                            category = 802; // Furniture
                            break;
                        case 3:
                            category = 803; // Electronic
                            break;
                        case 4:
                            category = 804; // Sport
                            break;
                        case 5:
                            category = 805; // Others
                            break;

                        default:
                            cout << RED << "Invalid category selection!" << RESET << endl;
                            return; // Exit the function or handle the error as needed

                        }

                        PRICE:
                        cout << YELLOW << "Enter Price (e.g., 5.00, 10.99 - Minimum RM 1.00): " << RESET << WHITE << "RM " << RESET;
                        cin >> price;
                        if (cin.fail() || cin.peek() != '\n') { // Handle invalid input (e.g., alphabetic characters)
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << RED << "Invalid input! Please enter a numeric value." << RESET << endl;
                            goto PRICE;
                        }

                        while (price < 1.00) { // Validate price input
                            cout << RED << "Invalid price! Please enter a positive value: " << RESET;
                            cin >> price;
                        }

                        cin.ignore();
                        cout << YELLOW << "Enter Description: " << RESET;
                        getline(cin, description);

                        system("cls");
                        cout << GREEN << "========================================" << RESET << endl;
                        cout << CYAN << "               ADD ITEM                " << RESET << endl;
                        cout << GREEN << "========================================" << RESET << endl;
                        cout << YELLOW << "Item Name: " << RESET << BWHITE << name << RESET << endl;

                        string categoryName;
                        switch (category) {
                        case 801:
                            categoryName = "Book";
                            break;
                        case 802:
                            categoryName = "Furniture";
                            break;
                        case 803:
                            categoryName = "Electronic";
                            break;
                        case 804:
                            categoryName = "Sport";
                            break;
                        case 805:
                            categoryName = "Others";
                            break;

                        default:
                            categoryName = "Unknown"; // In case of an invalid category number
                            break;
                        }
                        cout << YELLOW << "Category: " << RESET << BWHITE << categoryName << RESET << endl;

                        cout << YELLOW << "Price: " << RESET << BWHITE << "RM" << price << RESET << endl;
                        cout << YELLOW << "Description: " << RESET << BWHITE << description << RESET << endl;

                        // Asking Sure or NOT add the item    
                        char choice;
                        while (true) { // Start an infinite loop to keep asking for input
                            cout << CYAN << "Are you sure you want to add this item? (Y/N): " << RESET;
                            cin >> choice;  // Read only the first character
                            if (cin.peek() != '\n') {
                                // If there are extra characters, ignore the rest and consider it invalid
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                cout << RED << "Invalid choice! Please enter only 'Y' or 'N'." << RESET << endl;
                                continue;  // Prompt again
                            }

                            // Check if the input is valid (Y/y or N/n)
                            if (choice == 'Y' || choice == 'y') {
                                item.addItem(userid, name, description, price, category);
                                system("cls");
                                cout << CYAN << "( Item added successfully! )" << RESET << endl;
                                break; // Exit the loop if the user confirms the addition
                            }
                            else if (choice == 'N' || choice == 'n') {
                                system("cls");
                                cout << YELLOW << "Cancelled. Returning to menu..." << RESET << endl;
                                break; // Exit the loop if the user cancels the action
                            }
                            else {
                                // Invalid choice, print error and prompt again
                                cout << RED << "Invalid choice! Please enter 'Y' to confirm or 'N' to cancel." << RESET << endl;
                            }

                        }  break;

                    }
                }

                //Delete Item
                else if (manageItemChoice == 2) {
                    int itemid;
                    db.prepareStatement("SELECT * FROM item");
                    db.QueryResult();

                    if (db.res->next()) {
                        db.prepareStatement("SELECT * FROM item WHERE User_ID= ? AND Item_Status = 'Available'");
                        db.stmt->setInt(1, userid);
                        db.QueryResult();

                        if (db.res->next()) {
                            //display item list first
                            system("cls");
                            item.displayUserAvailableItemList(userid);
                            cout << endl << endl;

                            //Select Item to delete
                            while (true) {
                                cout << YELLOW << "Enter Item ID to delete (or enter 0 to return menu): " << RESET;
                                cin >> itemid;

                                // Check if input is numeric
                                if (cin.fail() || cin.peek() != '\n') {
                                    cin.clear(); // Clear error flag
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                                    cout << RED << "Invalid input! Please enter a valid numeric Item ID." << RESET << endl;
                                    continue;
                                }

                                if (itemid == 0) {
                                    system("cls");
                                    cout << BYELLOW << "Returning to menu..." << RESET << endl;
                                    break; // Exit the delete process entirely
                                }

                                // Check if Item ID exists in the database for this user
                                db.prepareStatement("SELECT * FROM item WHERE Item_ID = ? AND User_ID = ? AND Item_Status = 'Available'");
                                db.stmt->setInt(1, itemid);
                                db.stmt->setInt(2, userid);
                                db.QueryResult();

                                if (db.res->next()) {
                                    char choice;
                                    while (true) { // Start an infinite loop to keep asking for input
                                        cout << CYAN << endl << "Are you sure you want to delete this item? (Y/N): " << RESET;
                                        cin >> choice;  // Read only the first character
                                        if (cin.peek() != '\n') {
                                            // If there are extra characters, ignore the rest and consider it invalid
                                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                            cout << RED << "Invalid choice! Please enter only 'Y' or 'N'." << RESET << endl;
                                            continue;  // Prompt again
                                        }

                                        // Check if the input is valid (Y/y or N/n)
                                        if (choice == 'Y' || choice == 'y') {
                                            item.removeItem(itemid, userid);
                                            system("cls");
                                            cout << CYAN << "( Item deleted successfully! )" << RESET << endl;
                                            break; // Exit the loop if the user confirms the addition
                                        }
                                        else if (choice == 'N' || choice == 'n') {
                                            system("cls");
                                            cout << YELLOW << "Cancelled. Returning to menu..." << RESET << endl;
                                            break; // Exit the loop if the user cancels the action
                                        }
                                        else {
                                            // Invalid choice, print error and prompt again
                                            cout << RED << "Invalid choice! Please enter 'Y' to confirm or 'N' to cancel." << RESET << endl;
                                        }
                                    }
                                    // Valid Item ID found
                                    break;
                                }
                                else {
                                    cout << RED << "Item ID does not exist or be Sold. Please try again." << RESET << endl;
                                }
                            }


                        }

                        else {
                            system("cls");
                            cout << RED << "You don't have any items!" << RESET << endl;
                        }


                    }
                    else {
                        cout << "No items available!" << endl;

                    }
                }

                //Update Item
                else if (manageItemChoice == 3) {
                    int itemid, choice;
                    string itemName, itemDescription;
                    double itemPrice;
                    db.prepareStatement("SELECT * FROM item");
                    db.QueryResult();

                    if (db.res->next()) {
                        db.prepareStatement("SELECT * FROM item WHERE User_ID= ? AND Item_Status = 'Available'");
                        db.stmt->setInt(1, userid);
                        db.QueryResult();

                        if (db.res->next()) {
                            //dispaly User Item list
                            system("cls");
                            item.displayUserAvailableItemList(userid);
                            cout << endl << endl;
                            cout << string(30, '-') << endl;
                            cout << YELLOW << "   Choose a Item to Edit" << RESET << endl;
                            cout << string(30, '-') << endl;
                            while (true) {
                                cout << YELLOW << "Enter Item ID to edit (or enter 0 to return menu): " << RESET;
                                cin >> itemid;

                                // Check if input is numeric
                                if (cin.fail() || cin.peek() != '\n') {
                                    cin.clear(); // Clear error flag
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                                    cout << RED << "Invalid input! Please enter a valid numeric Item ID." << RESET << endl;
                                    continue;
                                }

                                if (itemid == 0) {
                                    system("cls");
                                    cout << BYELLOW << "Returning to menu..." << RESET << endl;
                                    break; // Exit the delete process entirely
                                }

                                // Check if Item ID exists in the database for this user
                                db.prepareStatement("SELECT * FROM item WHERE Item_ID = ? AND User_ID = ?");
                                db.stmt->setInt(1, itemid);
                                db.stmt->setInt(2, userid);
                                db.QueryResult();

                                if (db.res->next()) {
                                    system("cls");
                                    while (true) {
                                        cout << GREEN << "========================================" << RESET << endl;
                                        cout << CYAN << "               EDIT ITEM                " << RESET << endl;
                                        cout << GREEN << "========================================" << RESET << endl << endl;
                                        cout << BWHITE << "1. Item Name" << endl;
                                        cout << "2. Item Description" << endl;
                                        cout << "3. Item Price" << endl;
                                        cout << "0. Return to menu" << RESET << endl;

                                        cout << BYELLOW << endl << "Enter your choice:" << RESET;
                                        cin >> choice;

                                        if (cin.fail() || cin.peek() != '\n') {
                                            cin.clear(); // Clear the error flag
                                            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                                            system("cls");
                                            cout << RED << "Invalid input! Please enter a numeric choice." << RESET << endl;
                                            continue;
                                        }
                                        //Edit Item Name
                                        if (choice == 1) {
                                            system("cls");
                                            cout << GREEN << "========================================" << RESET << endl;
                                            cout << CYAN << "             EDIT ITEM NAME              " << RESET << endl;
                                            cout << GREEN << "========================================" << RESET << endl << endl;

                                            // Retrieve current item name
                                            db.prepareStatement("SELECT Item_Name FROM item WHERE Item_ID = ? AND User_ID = ?");
                                            db.stmt->setInt(1, itemid);
                                            db.stmt->setInt(2, userid);
                                            db.QueryResult();

                                            string currentName;
                                            if (db.res->next()) {
                                                currentName = db.res->getString("Item_Name");
                                            }
                                            else {
                                                cout << RED << "Failed to retrieve the current Item Name." << RESET << endl;
                                                break; // Exit the loop if the current name cannot be retrieved
                                            }

                                            // Display the current name
                                            cout << YELLOW << "Current Item Name: " << RESET << CYAN << "\"" << currentName << "\"" << RESET << endl << endl;

                                            cout << YELLOW << "Enter New Item Name (enter '0' for return menu): " << RESET;
                                            cin.ignore();
                                            getline(cin, itemName);
                                            cout << endl;

                                            if (itemName == "0") {
                                                system("cls");
                                                cout << YELLOW << "Returning to Edit Item Menu..." << RESET << endl;
                                            }
                                            else if (itemName.empty() || all_of(itemName.begin(), itemName.end(), isspace)) {
                                                system("cls");
                                                cout << RED << "Error: Item Name cannot be empty or contain only spaces!" << RESET << endl;
                                            }
                                            else {  // Asking Sure or NOT add the item    
                                                char choice;
                                                while (true) { // Start an infinite loop to keep asking for input
                                                    cout << CYAN << "Are you sure you want to edit this item name? (Y/N): " << RESET;
                                                    cin >> choice;  // Read only the first character
                                                    if (cin.peek() != '\n') {
                                                        // If there are extra characters, ignore the rest and consider it invalid
                                                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                                        cout << RED << "Invalid choice! Please enter only 'Y' or 'N'." << RESET << endl;
                                                        continue;  // Prompt again
                                                    }

                                                    // Check if the input is valid (Y/y or N/n)
                                                    if (choice == 'Y' || choice == 'y') {
                                                        db.prepareStatement("UPDATE item SET Item_Name = ? WHERE Item_ID = ? AND User_ID = ?");
                                                        db.stmt->setString(1, itemName);
                                                        db.stmt->setInt(2, itemid);
                                                        db.stmt->setInt(3, userid);
                                                        db.stmt->executeUpdate();
                                                        system("cls");
                                                        cout << CYAN << "( Item Updated successfully! )" << RESET << endl;
                                                        break; // Exit the loop if the user confirms the addition
                                                    }
                                                    else if (choice == 'N' || choice == 'n') {
                                                        system("cls");
                                                        cout << YELLOW << "Cancelled. Returning to menu..." << RESET << endl;
                                                        break; // Exit the loop if the user cancels the action
                                                    }
                                                    else {
                                                        // Invalid choice, print error and prompt again
                                                        cout << RED << "Invalid choice! Please enter 'Y' to confirm or 'N' to cancel." << RESET << endl;
                                                    }
                                                }
                                            }

                                        }
                                        //Edit Item Description
                                        else if (choice == 2) {
                                            system("cls");
                                            cout << GREEN << "========================================" << RESET << endl;
                                            cout << CYAN << "          EDIT ITEM DESCRIPTION              " << RESET << endl;
                                            cout << GREEN << "========================================" << RESET << endl << endl;

                                            // Retrieve current item description
                                            db.prepareStatement("SELECT Item_Description FROM item WHERE Item_ID = ? AND User_ID = ?");
                                            db.stmt->setInt(1, itemid);
                                            db.stmt->setInt(2, userid);
                                            db.QueryResult();

                                            string currentDescription;
                                            if (db.res->next()) {
                                                currentDescription = db.res->getString("Item_Description");
                                            }
                                            else {
                                                cout << RED << "Failed to retrieve the current Item Description." << RESET << endl;
                                                break; // Exit the loop if the current name cannot be retrieved
                                            }

                                            cout << YELLOW << "Current Item Description: " << RESET << CYAN << "\"" << currentDescription << "\"" << RESET << endl << endl;
                                            cout << YELLOW << "Enter New Item Description (enter '0' for return menu): " << RESET;
                                            cin.ignore();
                                            getline(cin, itemDescription);
                                            cout << endl;
                                            if (itemDescription == "0") {
                                                system("cls");
                                                cout << YELLOW << "Returning to Edit Item Menu..." << RESET << endl;
                                            }
                                            else {  // Asking Sure or NOT add the item    
                                                char choice;
                                                while (true) { // Start an infinite loop to keep asking for input
                                                    cout << CYAN << "Are you sure you want to edit this item description? (Y/N): " << RESET;
                                                    cin >> choice;  // Read only the first character
                                                    if (cin.peek() != '\n') {
                                                        // If there are extra characters, ignore the rest and consider it invalid
                                                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                                        cout << RED << "Invalid choice! Please enter only 'Y' or 'N'." << RESET << endl;
                                                        continue;  // Prompt again
                                                    }

                                                    // Check if the input is valid (Y/y or N/n)
                                                    if (choice == 'Y' || choice == 'y') {
                                                        db.prepareStatement("UPDATE item SET Item_Description = ? WHERE Item_ID = ? AND User_ID = ?");
                                                        db.stmt->setString(1, itemDescription);
                                                        db.stmt->setInt(2, itemid);
                                                        db.stmt->setInt(3, userid);
                                                        db.stmt->executeUpdate();
                                                        system("cls");
                                                        cout << CYAN << "( Item updated successfully! )" << RESET << endl;
                                                        break; // Exit the loop if the user confirms the addition
                                                    }
                                                    else if (choice == 'N' || choice == 'n') {
                                                        system("cls");
                                                        cout << YELLOW << "Cancelled. Returning to menu..." << RESET << endl;
                                                        break; // Exit the loop if the user cancels the action
                                                    }
                                                    else {
                                                        // Invalid choice, print error and prompt again
                                                        cout << RED << "Invalid choice! Please enter 'Y' to confirm or 'N' to cancel." << RESET << endl;
                                                    }
                                                    break;
                                                }
                                            }

                                        }

                                        else if (choice == 3) {
                                            system("cls");
                                            cout << GREEN << "========================================" << RESET << endl;
                                            cout << CYAN << "              EDIT ITEM PRICE              " << RESET << endl;
                                            cout << GREEN << "========================================" << RESET << endl << endl;

                                            // Retrieve current item description
                                            db.prepareStatement("SELECT Item_Price FROM item WHERE Item_ID = ? AND User_ID = ?");
                                            db.stmt->setInt(1, itemid);
                                            db.stmt->setInt(2, userid);
                                            db.QueryResult();

                                            string currentPrice;
                                            if (db.res->next()) {
                                                currentPrice = db.res->getString("Item_Price");
                                            }
                                            else {
                                                cout << RED << "Failed to retrieve the current Item Description." << RESET << endl;
                                                break; // Exit the loop if the current name cannot be retrieved
                                            }

                                            cout << YELLOW << "Current Item Price: " << RESET << CYAN << "\"RM " << currentPrice << "\"" << RESET << endl << endl;
                                            cout << YELLOW << "Enter new item price (e.g., 5.00, 10.99 - Minimum RM 1.00) (enter '0' for return menu): " << RESET;
                                            cin >> itemPrice;
                                            cout << endl;

                                            if (cin.fail() || cin.peek() != '\n') { // Handle invalid input (e.g., alphabetic characters)
                                                cin.clear();
                                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                                system("cls");
                                                cout << RED << "Invalid input! Please try again!." << RESET << endl;
                                                continue;
                                            }

                                            if (itemPrice == 0) { // Handle '0' to return to menu
                                                system("cls");
                                                cout << YELLOW << "Returning to Edit Item Menu..." << RESET << endl;
                                                continue;
                                            }

                                            if (itemPrice >= 1.00) { // Valid pricechar choice;
                                                char choice;
                                                while (true) { // Start an infinite loop to keep asking for input
                                                    cout << CYAN << "Are you sure you want to edit this item price? (Y/N): " << RESET;
                                                    cin >> choice;  // Read only the first character
                                                    if (cin.peek() != '\n') {
                                                        // If there are extra characters, ignore the rest and consider it invalid
                                                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                                        cout << RED << "Invalid choice! Please enter only 'Y' or 'N'." << RESET << endl;
                                                        continue;  // Prompt again
                                                    }

                                                    // Check if the input is valid (Y/y or N/n)
                                                    if (choice == 'Y' || choice == 'y') {
                                                        // Update the item price
                                                        db.prepareStatement("UPDATE item SET Item_Price = ? WHERE Item_ID = ? AND User_ID = ?");
                                                        db.stmt->setDouble(1, itemPrice);
                                                        db.stmt->setInt(2, itemid);
                                                        db.stmt->setInt(3, userid);
                                                        db.stmt->executeUpdate();

                                                        system("cls");
                                                        cout << CYAN << "( Item price updated successfully! )" << RESET << endl;
                                                        break;
                                                    }
                                                    else if (choice == 'N' || choice == 'n') {
                                                        system("cls");
                                                        cout << YELLOW << "Cancelled. Returning to menu..." << RESET << endl;
                                                        break; // Exit the loop if the user cancels the action
                                                    }
                                                    else {
                                                        // Invalid choice, print error and prompt again
                                                        cout << RED << "Invalid choice! Please enter 'Y' to confirm or 'N' to cancel." << RESET << endl;
                                                    }
                                                }
                                            }
                                            else {
                                                cout << RED << "Invalid price! Please enter a value of 1.00 or higher." << RESET << endl;
                                            }
                                        }

                                        else if (choice == 0) {
                                            system("cls");
                                            cout << YELLOW << "Returning to user menu..." << RESET << endl;
                                            break;
                                        }
                                        else {
                                            system("cls");
                                            cout << RED << "Invalid input! Please enter a numeric choice." << RESET << endl;

                                        }
                                    }
                                    // Valid Item ID found
                                    break;
                                }
                                else {
                                    cout << RED << "Item ID does not exist. Please try again." << RESET << endl;
                                }
                            }

                            //Edit

                        }
                        else {
                            system("cls");
                            cout << RED << "You don't have any items!" << RESET << endl;

                        }

                    }
                }

                //Display Item
                else if (manageItemChoice == 4) {
                    int menuChoice = -1;
                    int detailChoice;
                    int itemID;

                    db.prepareStatement("SELECT * FROM item WHERE User_ID = ? AND Item_Status = 'Available'");
                    db.stmt->setInt(1, userid);
                    db.QueryResult();

                    if (db.res->next()) {
                        while (menuChoice != 0) { // Loop until the user decides to return to the main user menu
                            system("cls");
                            item.displayUserAvailableItemList(userid); // Display the user's item list
                            cout << endl << endl << endl << endl;

                            cout << YELLOW << "===Menu=== " << RESET << endl;
                            cout << BWHITE << "1. See Item's Details " << RESET << endl;
                            cout << BWHITE << "0. Return to user menu" << RESET << endl << endl;
                            ENTERCHOICE:
                            cout << BYELLOW << "Enter your choice: " << RESET;
                            cin >> menuChoice;

                            if (cin.fail() || cin.peek() != '\n') {
                                cin.clear(); // Clear the error flag
                                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                                cout << RED << "Invalid input! Please enter a numeric choice." << RESET << endl;
                                goto ENTERCHOICE;
                            }

                            if (menuChoice == 1) {
                                cout << BYELLOW << "Enter Item ID to see Item's Details: " << RESET;
                                cin >> itemID;

                                if (cin.fail() || cin.peek() != '\n') {
                                    cin.clear(); // Clear error flag
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                                    cout << RED << "Invalid input! Please enter a valid numeric Item ID." << RESET << endl;
                                    continue;
                                }

                                item.displayItemDetails(itemID, userid);

                                cout << endl << endl << endl << BYELLOW << "Press any key to return to item list..." << RESET;
                                _getch();  // Wait for a key press
                                system("cls");
                            }
                          
                            else if (menuChoice == 0) {
                                system("cls");
                                cout << BYELLOW << "Returning to menu..." << RESET << endl;
                            }  
                            else {
                                cout << RED << "Invalid choice. Please try again." << RESET << endl;
                                goto ENTERCHOICE;
                            }
                        }
                    }
                    else {
                        // If no items are available, display an error and exit this option
                        system("cls");
                        cout << RED << "No items available!" << RESET << endl;
                    }
                }

                //Return to menu
                else if (manageItemChoice == 0) {
                    system("cls");
                    cout << YELLOW << "Returning to menu..." << RESET << endl;
                    goto userMenu;
                } 
                else {
                    system("cls");
                    cout << RED << "Invalid input! Please enter a numeric choice." << RESET << endl;
                }
            }
        }

        //Buy Item
        else if (userChoice == 2) {
        BuyMenu:
            int itemID;
            int buyerID = userid;
            int selectFunction;
            db.prepareStatement("SELECT * FROM item");
            db.QueryResult();

            if (db.res->next()) {
                system("cls");
                item.displayAllItemList();
                cout << endl << endl << endl << endl;
                cout << BYELLOW << "===Menu===" << RESET << endl;
                cout << BWHITE << "1. Buy Item" << RESET << endl;
                cout << BWHITE << "2. Search and Filter Item" << RESET << endl;
                cout << BWHITE << "0. Return to Menu" << RESET << endl << endl;
                BUYCHOICE:
                cout << BYELLOW << "Enter your choice: " << RESET;
                cin >> selectFunction;

                if (cin.fail() || cin.peek() != '\n') {
                    cin.clear(); // Clear error flag
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                    cout << RED << "Invalid input! Please try again!" << RESET << endl;
                    goto BUYCHOICE;
                }

                 //Straight to Buy Item
                if (selectFunction == 1) {
                    cout << YELLOW << "Enter Item ID to buy: " << RESET;
                    cin >> itemID;

                    if (cin.fail() || cin.peek() != '\n') {
                        cin.clear(); // Clear error flag
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                        cout << RED << "Invalid input! Please enter a valid numeric Item ID." << RESET << endl;
                        goto BUYCHOICE;
                    }

                    // Check if the Item_ID exists or not
                    DBConnection db;
                    db.prepareStatement("SELECT Item_ID FROM item WHERE Item_ID = ?");
                    db.stmt->setInt(1, itemID);
                    db.QueryResult();

                    if (!db.res->next()) { // No matching Item_ID found
                        cout << RED << "No item found with Item ID: " << itemID << ". Please try again." << RESET << endl;
                        goto BUYCHOICE; // Restart the loop
                    }

                    item.buyItem(userid, itemID);
                }
                //Search Item first then buy Item
                else if (selectFunction == 2) {
                    string keyword;
                    double minPrice = -1, maxPrice = -1;
                    int categoryID = -1;
                    system("cls");
                    cout << BYELLOW << "===== Search and Filter Menu =====" << RESET << endl;

                    // Get keyword input
                    cout << BWHITE << "Enter keyword to search (or leave blank to skip): " << RESET;
                    cin.ignore(); // Clear the input buffer
                    getline(cin, keyword);

                    // Get price range input
                    while (true) {
                        cout << BWHITE << "Enter minimum price (or -1 to skip): " << RESET;
                        cin >> minPrice;

                        if (cin.fail() || minPrice < -1) { // Ensure valid input
                            cin.clear(); // Clear the error flag
                            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                            cout << RED << "Invalid input! Please enter a numeric value greater than or equal to -1." << RESET << endl;
                        }
                        else {
                            break; // Valid input
                        }
                    }

                    while (true) {
                        cout << BWHITE << "Enter maximum price (or -1 to skip): " << RESET;
                        cin >> maxPrice;

                        if (cin.fail() || maxPrice < -1 || cin.peek()!='\n') { // Ensure valid input
                            cin.clear(); // Clear the error flag
                            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                            cout << RED << "Invalid input! Please enter a numeric value greater than or equal to -1." << RESET << endl;
                        }
                        else if (maxPrice != -1 && minPrice != -1 && maxPrice < minPrice) {
                            cout << RED << "Invalid input! Maximum price cannot be less than minimum price." << RESET << endl;
                        }
                        else {
                            break; // Valid input
                        }
                    }

                    // Get category input
                    while (true) {
                        cout << BWHITE << "Select Category:" << endl;
                        cout << CYAN << "   1. Book" << endl;
                        cout << "   2. Furniture" << endl;
                        cout << "   3. Electronic" << endl;
                        cout << "   4. Sport" << endl;
                        cout << "   5. Others" << RESET << endl;
                        cout << BWHITE << "Enter category to filter by (or -1 to skip): " << RESET;
                        cin >> categoryID;

                        if (cin.fail() || (categoryID != -1 && (categoryID < 1 || categoryID > 5)) || cin.peek() != '\n') { // Ensure valid input
                            cin.clear(); // Clear the error flag
                            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                            cout << RED << "Invalid input! Please enter a number between 1 and 5, or -1 to skip." << RESET << endl;
                        }
                        else {
                            break; // Valid input
                        }
                    }

                    // Map category selection to IDs
                    switch (categoryID) {
                    case 1:
                        categoryID = 801; // Book
                        break;
                    case 2:
                        categoryID = 802; // Furniture
                        break;
                    case 3:
                        categoryID = 803; // Electronic
                        break;
                    case 4:
                        categoryID = 804; // Sport
                        break;
                    case 5:
                        categoryID = 805; // Others
                        break;
                    case -1:
                        categoryID = -1; // Skip category filtering
                        break;
                    default:
                        // Should not reach here due to error handling above
                        break;
                    }
                    // Call the search function in Item
                    system("cls");
                    item.SearchAndFilter(keyword, minPrice, maxPrice, categoryID);
                    while (true) {
                        cout << YELLOW << "Enter Item ID to buy (or enter 0 to cancel): " << RESET;
                        cin >> itemID;

                        if (cin.fail() || cin.peek() != '\n') {
                            cin.clear(); // Clear error flag
                            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                            cout << RED << "Invalid input! Please enter a valid numeric Item ID or 0 to cancel." << RESET << endl;
                            continue;
                        }

                        if (itemID == 0) { // Check if the user wants to quit
                            system("cls");
                            cout << BYELLOW << "Exiting purchase process..." << RESET << endl;
                            break;
                        }


                        // Check if the Item_ID exists in the user's inventory
                        DBConnection db;
                        db.prepareStatement("SELECT Item_ID FROM item WHERE Item_ID = ?");
                        db.stmt->setInt(1, itemID);
                        db.QueryResult();

                        if (!db.res->next()) { // No matching Item_ID found
                            cout << RED << "No item found with Item ID: " << itemID << ". Please try again or enter 0 to cancel." << RESET << endl;
                            continue; // Restart the loop
                        }

                        item.buyItem(userid, itemID);
                        break; // Exit the loop after successful purchase
                    } 
                    

                }
               
                //Return to menu
                else if (selectFunction == 0) {
                    system("cls");
                    cout << BYELLOW << "Returning to user menu..." << RESET << endl;
                    goto userMenu;
                }
                else {
                    cout << RED << "Invalid input! Please try again!" << RESET << endl;
                    goto BUYCHOICE;
                }
            }
        }

        //Manage transaction Status
        else if (userChoice == 3) {
            int menuChoice = -1;
            int detailChoice;
            int transactionID;

            db.prepareStatement("SELECT * FROM item WHERE User_ID = ?");
            db.stmt->setInt(1, userid);
            db.QueryResult();

            if (db.res->next()) {
                while (menuChoice != 0) { // Loop until the user decides to return to the main user menu
                    system("cls");
                    item.ViewPendingTransactions(userid); // Display the user's item list
                    cout << endl << endl << endl << endl;
                    cout << YELLOW << "===Menu=== " << RESET << endl;
                    cout << BWHITE << "1. Change Order Status " << RESET << endl;
                    cout << BWHITE << "0. Return to user menu" << RESET << endl << endl;
                    TRANSACTIONCHOICE:
                    cout << BYELLOW << "Enter your choice: " << RESET;
                    cin >> menuChoice;

                    if (cin.fail() || cin.peek() != '\n') {
                        cin.clear(); // Clear the error flag
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                        cout << RED << "Invalid input! Please enter a numeric choice." << RESET << endl;
                        goto TRANSACTIONCHOICE;
                    }


                    if (menuChoice == 1) {
                        cout << BYELLOW << "Enter " << BCYAN << "Transaction ID "<<BYELLOW << "to change Order Status : " << RESET;
                        cin >> transactionID;

                        if (cin.fail() || cin.peek() != '\n') {
                            cin.clear(); // Clear error flag
                            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                            cout << RED << "Invalid input! Please try again!." << RESET << endl;
                            continue;
                        }
                        item.displayTransactionItemDetails(transactionID, userid);
                        item.ConfirmTransactionCompletion(transactionID, userid);

                        cout << endl << endl << endl << BYELLOW << "Press any key to return to item list..." << RESET;
                        _getch();  // Wait for a key press
                        system("cls");
                    }
                    else if (menuChoice == 0) {
                        system("cls");
                        cout << BYELLOW << "Returning to menu..." << RESET << endl;
                    }
                    else {
                        cout << RED << "Invalid input! Please try again!" << RESET << endl;
                        goto TRANSACTIONCHOICE;
                    }
                }
            }

            else {
                // If no items are available, display an error and exit this option
                system("cls");
                cout << RED << "No Any Order Avaiable!" << RESET << endl;
            }
        }
        
        //MyPurchase
        else if (userChoice == 4) {
            int menuChoice = -1;
            int detailChoice;
            int itemID;

            db.prepareStatement("SELECT * FROM item WHERE User_ID = ? ");
            db.stmt->setInt(1, userid);
            db.QueryResult();

            if (db.res->next()) {
                while (menuChoice != 0) { // Loop until the user decides to return to the main user menu
                    system("cls");
                    item.displayUserPurchaseItemList(userid); // Display the user's item list
                    cout << endl << endl << endl << endl;
                    cout << YELLOW << "===Menu=== " << RESET << endl;
                    cout << BWHITE << "1. See Item's Details " << RESET << endl;
                    cout << BWHITE << "0. Return to user menu" << RESET << endl << endl;
                    PURCHASECHOICE:
                    cout << BYELLOW << "Enter your choice: " << RESET;
                    cin >> menuChoice;

                    if (cin.fail() || cin.peek() != '\n') {
                        cin.clear(); // Clear the error flag
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                        cout << RED << "Invalid input! Please enter a numeric choice." << RESET << endl;
                        goto PURCHASECHOICE;
                    }

                    if (menuChoice == 1) {
                        cout << BYELLOW << "Enter Transaction ID to see Transaction's Details: " << RESET;
                        cin >> itemID;

                        if (cin.fail() || cin.peek() != '\n') {
                            cin.clear(); // Clear error flag
                            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                            cout << RED << "Invalid input! Please enter a valid numeric Item ID." << RESET << endl;
                            goto PURCHASECHOICE;
                        }

                        item.displayPurchaseItemDetails(itemID, userid);

                        cout << endl << endl << endl << BYELLOW << "Press any key to return to item list..." << RESET;
                        _getch();  // Wait for a key press
                        system("cls");
                    }
                    else if (menuChoice == 0) {
                        system("cls");
                        cout << BYELLOW << "Returning to user menu..." << RESET << endl;
                    }
                    else {
                        cout << RED << "Invalid choice. Please try again." << RESET << endl;
                        goto PURCHASECHOICE;
                    }
                }

            }
            else {
                // If no items are available, display an error and exit this option
                system("cls");
                cout << RED << "No Items Purchase!" << RESET << endl;
            }
        }

        //MySales
        else if (userChoice == 5) {
            int menuChoice = -1;
            int detailChoice;
            int itemID;

            db.prepareStatement("SELECT * FROM item WHERE User_ID = ? AND Item_Status = 'Sold'");
            db.stmt->setInt(1, userid);
            db.QueryResult();

            if (db.res->next()) {
                while (menuChoice != 0) { // Loop until the user decides to return to the main user menu
                    system("cls");
                    item.displayUserSoldItemList(userid); // Display the user's item list
                    cout << endl << endl << endl << endl;
                    cout << YELLOW << "===Menu=== " << RESET << endl;
                    cout << BWHITE << "1. See Item's Details " << RESET << endl;
                    cout << BWHITE << "0. Return to user menu" << RESET << endl << endl;
                    SALESCHOICE:
                    cout << BYELLOW << "Enter your choice: " << RESET;
                    cin >> menuChoice;

                    if (cin.fail() || cin.peek() != '\n') {
                        cin.clear(); // Clear the error flag
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                        cout << RED << "Invalid input! Please enter a numeric choice." << RESET << endl;
                        goto SALESCHOICE;
                    }

                    if (menuChoice == 1) {
                        cout << BYELLOW << "Enter Transaction ID to see Transaction's Details: " << RESET;
                        cin >> itemID;

                        if (cin.fail() || cin.peek() != '\n') {
                            cin.clear(); // Clear error flag
                            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                            cout << RED << "Invalid input! Please enter a valid numeric Item ID." << RESET << endl;
                            goto SALESCHOICE;
                        }

                        item.displaySoldItemDetails(itemID, userid);

                        cout << endl << endl << endl << BYELLOW << "Press any key to return to item list..." << RESET;
                        _getch();  // Wait for a key press
                        system("cls");
                    }
                    else if (menuChoice == 0) {
                        system("cls");
                        cout << BYELLOW << "Returning to user menu..." << RESET << endl;
                    }
                    else {
                        cout << RED << "Invalid choice. Please try again." << RESET << endl;
                        goto SALESCHOICE;
                    }
                }
            }
            else {
                // If no items are available, display an error and exit this option
                system("cls");
                cout << RED << "No Items SOLD!" << RESET << endl;
            }
        }

        //MyProfile
        else if (userChoice == 6) {
            system("cls");
            while (true) {
                int choice;
                item.MyProfile(userid);
                cout << endl << endl;
                cout << BYELLOW << "===Menu===" << RESET << endl;
                cout << BWHITE << "1. Edit Profile" << endl;
                cout << "0. Return to menu" << RESET << endl;
                EDITPROFILECHOICE:
                cout << BYELLOW << endl << "Enter your choice: " << RESET;
                cin >> choice;

                if (cin.fail() || cin.peek() != '\n') {
                    cin.clear(); // Clear error flag
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                    cout << RED << "Invalid input! Please try again!." << RESET << endl;
                    goto EDITPROFILECHOICE;
                }

                if (choice == 1) {
                    int editChoice;
                    system("cls");
                    cout << GREEN << "========================================" << RESET << endl;
                    cout << CYAN << "           EDIT USER PROFILE                " << RESET << endl;
                    cout << GREEN << "========================================" << RESET << endl << endl;
                    cout << BWHITE << "1. Change Password" << endl;
                    cout << "2. Change Email" << endl;
                    cout << "3. Change Phone Number" << endl;
					cout << "4. Change Address" << endl;
                    cout << "0. Return to menu" << RESET << endl;
                    EDITUSERPROFILECHOICE:
                    cout << BYELLOW << endl << "Enter your choice: " << RESET;
                    cin >> editChoice;
                    if (cin.fail() || cin.peek() != '\n') {
                        cin.clear(); // Clear error flag
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                        cout << RED << "Invalid input! Please try again!." << RESET << endl;
                        goto EDITUSERPROFILECHOICE;
                    }

                    cin.ignore();
                    if (editChoice == 1) {
                        string oldPassword;
                        system("cls");
                        cout << BYELLOW << "Enter old password: " << RESET;
                        getline(cin, oldPassword);
                        item.UserEditPassword(userid, oldPassword);
                    }

                    else if (editChoice == 2) {
                        string newEmail;
                        system("cls");
                        cout << BYELLOW << "Enter new email: " << RESET;
                        getline(cin, newEmail);
                        item.UserEditEmail(userid, newEmail);
                    }

                    else if (editChoice == 3) {
                        string newPhone;
                        system("cls");
                        cout << BYELLOW << "Enter new phone number: " << RESET;
                        getline(cin, newPhone);
                        item.UserEditPhone(userid, newPhone);
                    }
					else if (editChoice == 4) {
						string newAddress;
						system("cls");
						cout << BYELLOW << "Enter new address: " << RESET;
						getline(cin, newAddress);
						item.UserEditAddress(userid, newAddress);
					}
                    else if (editChoice == 0) {
                        system("cls");
                        cout << YELLOW << "Returning to user profile..." << RESET << endl;
                    }


                }
                else if (choice == 0) {
                    system("cls");
                    cout << YELLOW << "Returning to user menu..." << RESET << endl;
                    goto userMenu;
                }
                else {
                    cout << RED << "Invalid Input. Please try again!" << RESET << endl;
                    goto EDITPROFILECHOICE;
                }
            }
        }
        
        //Logout
        else if (userChoice == 0) {
                system("cls");
                cout << YELLOW << "You have been logged out. Returning to main menu..." << RESET << endl;
                return;
        }

        //Invalid Choice
        else {
                system("cls");
                cout << RED << "Invalid choice! Please try again!" << RESET << endl;
        }
        
    }
}

    
//Admin Menu
void Menu::adminMenu() {
    DBConnection db;
    int adminChoice;
AdminMenu:
    while (true) {
        // Display user menu options
        cout << GREEN << "========================================" << RESET << endl;
        cout << CYAN << "               ADMIN MENU               " << RESET << endl;
        cout << GREEN << "========================================" << RESET << endl << endl;
        cout << BWHITE << "1. Display Available Item" << RESET << endl;
        cout << BWHITE << "2. Display Sold Item" << RESET << endl;
        cout << BWHITE << "3. Delete Item" << RESET << endl;
        cout << BWHITE << "4. View Report" << RESET << endl;
        cout << BWHITE << "0. Logout" << RESET << endl << endl;
        cout << YELLOW << "Enter your choice: " << RESET;
        cin >> adminChoice;

        if (cin.fail() || cin.peek() != '\n') {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            system("cls");
            cout << RED << "Invalid input! Please enter a numeric choice." << RESET << endl;
            continue;
        }

        //Display all Available Item list
        if (adminChoice == 1) {
            int detailChoice;
            int itemID;
            int menuChoice = -1;
            db.prepareStatement("SELECT * FROM item");
            db.QueryResult();

            if (db.res->next()) {
                while (menuChoice != 0) { // Loop until the user decides to return to the main user menu
                    system("cls");
                    item.displayAllItemList(); // Display the user's item list
                    cout << endl << endl << endl << endl;
                    cout << YELLOW << "===Menu=== " << RESET << endl;
                    cout << BWHITE << "1. See Item's Details " << RESET << endl;
					cout << BWHITE << "2. Search and Filter Item" << RESET << endl;
                    cout << BWHITE << "0. Return to menu" << RESET << endl << endl;
                    AVAILABLECHOICE:
                    cout << BYELLOW << "Enter your choice: " << RESET;
                    cin >> menuChoice;

                    if (cin.fail() || cin.peek() != '\n') {
                        cin.clear(); // Clear the error flag
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                        cout << RED << "Invalid input! Please try again!." << RESET << endl;
                        goto AVAILABLECHOICE;
                    }

                    if (menuChoice == 1) {
                        cout << BYELLOW << "Enter Item ID to see Item's Details: " << RESET;
                        cin >> itemID;

                        if (cin.fail() || cin.peek() != '\n') {
                            cin.clear(); // Clear error flag
                            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                            cout << RED << "Invalid input! Please enter a valid numeric Item ID." << RESET << endl;
                            continue;
                        }

                        item.AdminDisplayItemDetails(itemID);

                        cout << endl << endl << endl << BYELLOW << "Press any key to return to item list..." << RESET;
                        _getch();  // Wait for a key press
                        system("cls");
                    }
                    else if (menuChoice == 2) {
                        string keyword;
                        double minPrice = -1, maxPrice = -1;
                        int categoryID = -1;
                        system("cls");
                        cout << BYELLOW << "===== Search and Filter Menu =====" << RESET << endl;

                        // Get keyword input
                        cout << BWHITE << "Enter keyword to search (or leave blank to skip): " << RESET;
                        cin.ignore(); // Clear the input buffer
                        getline(cin, keyword);

                        // Get price range input
                        while (true) {
                            cout << BWHITE << "Enter minimum price (or -1 to skip): " << RESET;
                            cin >> minPrice;

                            if (cin.fail() || minPrice < -1 || cin.peek() != '\n') { // Ensure valid input
                                cin.clear(); // Clear the error flag
                                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                                cout << RED << "Invalid input! Please enter a numeric value greater than or equal to -1." << RESET << endl;
                            }
                            else {
                                break; // Valid input
                            }
                        }

                        while (true) {
                            cout << BWHITE << "Enter maximum price (or -1 to skip): " << RESET;
                            cin >> maxPrice;

                            if (cin.fail() || maxPrice < -1 || cin.peek() != '\n') { // Ensure valid input
                                cin.clear(); // Clear the error flag
                                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                                cout << RED << "Invalid input! Please enter a numeric value greater than or equal to -1." << RESET << endl;
                            }
                            else if (maxPrice != -1 && minPrice != -1 && maxPrice < minPrice) {
                                cout << RED << "Invalid input! Maximum price cannot be less than minimum price." << RESET << endl;
                            }
                            else {
                                break; // Valid input
                            }
                        }

                        // Get category input
                        while (true) {
                            cout << BWHITE << "Select Category:" << endl;
                            cout << CYAN << "   1. Book" << endl;
                            cout << "   2. Furniture" << endl;
                            cout << "   3. Electronic" << endl;
                            cout << "   4. Sport" << endl;
                            cout << "   5. Others" << RESET << endl;
                            cout << BWHITE << "Enter category to filter by (or -1 to skip): " << RESET;
                            cin >> categoryID;

                            if (cin.fail() || (categoryID != -1 && (categoryID < 1 || categoryID > 5)) || cin.peek() != '\n') { // Ensure valid input
                                cin.clear(); // Clear the error flag
                                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                                cout << RED << "Invalid input! Please enter a number between 1 and 5, or -1 to skip." << RESET << endl;
                            }
                            else {
                                break; // Valid input
                            }
                        }

                        // Map category selection to IDs
                        switch (categoryID) {
                        case 1:
                            categoryID = 801; // Book
                            break;
                        case 2:
                            categoryID = 802; // Furniture
                            break;
                        case 3:
                            categoryID = 803; // Electronic
                            break;
                        case 4:
                            categoryID = 804; // Sport
                            break;
                        case 5:
                            categoryID = 805; // Others
                            break;
                        case -1:
                            categoryID = -1; // Skip category filtering
                            break;
                        default:
                            // Should not reach here due to error handling above
                            break;
                        }
                        // Call the search function in Item
                        system("cls");
                        item.SearchAndFilter(keyword, minPrice, maxPrice, categoryID);
                        while (true) {
                            cout << YELLOW << "Enter Item ID to see details (or enter 0 to cancel): " << RESET;
                            cin >> itemID;

                            if (cin.fail() || cin.peek() != '\n') {
                                cin.clear(); // Clear error flag
                                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                                cout << RED << "Invalid input! Please enter a valid numeric Item ID or 0 to cancel." << RESET << endl;
                                continue;
                            }

                            if (itemID == 0) { // Check if the user wants to quit
                                system("cls");
                                cout << BYELLOW << "Exiting process..." << RESET << endl;
                                break;
                            }


                            // Check if the Item_ID exists in the inventory
                            DBConnection db;
                            db.prepareStatement("SELECT Item_ID FROM item WHERE Item_ID = ?");
                            db.stmt->setInt(1, itemID);
                            db.QueryResult();

                            if (!db.res->next()) { // No matching Item_ID found
                                cout << RED << "No item found with Item ID: " << itemID << ". Please try again or enter 0 to cancel." << RESET << endl;
                                continue; // Restart the loop
                            }

                            item.AdminDisplayItemDetails(itemID);
                            cout << endl << endl << endl << BYELLOW << "Press any key to return to item list..." << RESET;
                            _getch();  // Wait for a key press
                            system("cls");
                            break;
                        }
                    }
                      

                    else if(menuChoice == 0) {
                        system("cls");
                        cout << BYELLOW << "Returning to admin menu..." << RESET << endl;
                    }  

                    else {
                        cout << RED << "Invalid choice. Please try again." << RESET << endl;
                        goto AVAILABLECHOICE;
                    }
                }

            }
            else {
                // If no items are available, display an error and exit this option
                system("cls");
                cout << RED << "No items available!" << RESET << endl;
            }
        }
    
        //Display all Sold Item List
        else if (adminChoice == 2) {
            int detailChoice;
            int itemID;
            int menuChoice = -1;
            db.prepareStatement("SELECT * FROM item");
            db.QueryResult();

            if (db.res->next()) {
                while (menuChoice != 0) { // Loop until the user decides to return to the main user menu
                    system("cls");
                    item.displaySoldItemList(); // Display the sold item list
                    cout << endl << endl << endl << endl;
                    cout << YELLOW << "===Menu=== " << RESET << endl;
                    cout << BWHITE << "1. See Item's Details " << RESET << endl;
                    cout << BWHITE << "2. Search and Filter " << RESET << endl;
                    cout << BWHITE << "0. Return to menu" << RESET << endl << endl;
                SOLDCHOICE:
                    cout << BYELLOW << "Enter your choice: " << RESET;
                    cin >> menuChoice;

                    if (cin.fail() || cin.peek() != '\n') {
                        cin.clear(); // Clear the error flag
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                        cout << RED << "Invalid input! Please enter a numeric choice." << RESET << endl;
                        goto SOLDCHOICE;
                    }

                    if (menuChoice == 1) {
                        cout << BYELLOW << "Enter Item ID to see Item's Details: " << RESET;
                        cin >> itemID;

                        if (cin.fail() || cin.peek() != '\n') {
                            cin.clear(); // Clear error flag
                            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                            cout << RED << "Invalid input! Please enter a valid numeric Item ID." << RESET << endl;
                            continue;
                        }

                        item.AdminDisplaySoldItemDetails(itemID);

                        cout << BYELLOW << "Press any key to return to item list..." << RESET;
                        _getch();  // Wait for a key press
                        system("cls");
                    }

                    else if (menuChoice == 2) {
                        string keyword;
                        double minPrice = -1, maxPrice = -1;
                        int categoryID = -1;
                        system("cls");
                        cout << BYELLOW << "===== Search and Filter Menu =====" << RESET << endl;

                        // Get keyword input
                        cout << BWHITE << "Enter keyword to search (or leave blank to skip): " << RESET;
                        cin.ignore(); // Clear the input buffer
                        getline(cin, keyword);

                        // Get price range input
                        while (true) {
                            cout << BWHITE << "Enter minimum price (or -1 to skip): " << RESET;
                            cin >> minPrice;

                            if (cin.fail() || minPrice < -1 || cin.peek() != '\n') { // Ensure valid input
                                cin.clear(); // Clear the error flag
                                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                                cout << RED << "Invalid input! Please enter a numeric value greater than or equal to -1." << RESET << endl;
                            }
                            else {
                                break; // Valid input
                            }
                        }

                        while (true) {
                            cout << BWHITE << "Enter maximum price (or -1 to skip): " << RESET;
                            cin >> maxPrice;

                            if (cin.fail() || maxPrice < -1 || cin.peek() != '\n') { // Ensure valid input
                                cin.clear(); // Clear the error flag
                                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                                cout << RED << "Invalid input! Please enter a numeric value greater than or equal to -1." << RESET << endl;
                            }
                            else if (maxPrice != -1 && minPrice != -1 && maxPrice < minPrice) {
                                cout << RED << "Invalid input! Maximum price cannot be less than minimum price." << RESET << endl;
                            }
                            else {
                                break; // Valid input
                            }
                        }

                        // Get category input
                        while (true) {
                            cout << BWHITE << "Select Category:" << endl;
                            cout << CYAN << "   1. Book" << endl;
                            cout << "   2. Furniture" << endl;
                            cout << "   3. Electronic" << endl;
                            cout << "   4. Sport" << endl;
                            cout << "   5. Others" << RESET << endl;
                            cout << BWHITE << "Enter category to filter by (or -1 to skip): " << RESET;
                            cin >> categoryID;

                            if (cin.fail() || (categoryID != -1 && (categoryID < 1 || categoryID > 5)) || cin.peek() != '\n') { // Ensure valid input
                                cin.clear(); // Clear the error flag
                                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                                cout << RED << "Invalid input! Please enter a number between 1 and 5, or -1 to skip." << RESET << endl;
                            }
                            else {
                                break; // Valid input
                            }
                        }

                        // Map category selection to IDs
                        switch (categoryID) {
                        case 1:
                            categoryID = 801; // Book
                            break;
                        case 2:
                            categoryID = 802; // Furniture
                            break;
                        case 3:
                            categoryID = 803; // Electronic
                            break;
                        case 4:
                            categoryID = 804; // Sport
                            break;
                        case 5:
                            categoryID = 805; // Others
                            break;
                        case -1:
                            categoryID = -1; // Skip category filtering
                            break;
                        default:
                            // Should not reach here due to error handling above
                            break;
                        }
                        // Call the search function in Item
                        system("cls");
                        item.SearchAndFilterSoldItem(keyword, minPrice, maxPrice, categoryID);
                        while (true) {
                            cout << YELLOW << "Enter Item ID to see details (or enter 0 to cancel): " << RESET;
                            cin >> itemID;

                            if (cin.fail() || cin.peek() != '\n') {
                                cin.clear(); // Clear error flag
                                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                                cout << RED << "Invalid input! Please enter a valid numeric Item ID or 0 to cancel." << RESET << endl;
                                continue;
                            }

                            if (itemID == 0) { // Check if the user wants to quit
                                system("cls");
                                cout << BYELLOW << "Exiting process..." << RESET << endl;
                                break;
                            }


                            // Check if the Item_ID exists in the inventory
                            DBConnection db;
                            db.prepareStatement("SELECT Item_ID FROM item WHERE Item_ID = ?");
                            db.stmt->setInt(1, itemID);
                            db.QueryResult();

                            if (!db.res->next()) { // No matching Item_ID found
                                cout << RED << "No item found with Item ID: " << itemID << ". Please try again or enter 0 to cancel." << RESET << endl;
                                continue; // Restart the loop
                            }

                            item.AdminDisplaySoldItemDetails(itemID);
                            cout << endl << endl << endl << BYELLOW << "Press any key to return to item list..." << RESET;
                            _getch();  // Wait for a key press
                            system("cls");
                            break;
                        }
                    }

                    else if (menuChoice == 0) {
                        system("cls");
                        cout << BYELLOW << "Returning to admin menu..." << RESET << endl;
                    }

                    else {
                        cout << RED << "Invalid choice. Please try again." << RESET << endl;
                        goto SOLDCHOICE;
                    }
                    
                }

         
            }
            else {
                // If no items are available, display an error and exit this option
                system("cls");
                cout << RED << "No items available!" << RESET << endl;
            }
        }

        //Delete Item
        else if (adminChoice == 3) {
            int itemid;
            db.prepareStatement("SELECT * FROM item");
            db.QueryResult();

            if (db.res->next()) {
                db.prepareStatement("SELECT * FROM item ");
                db.QueryResult();

                if (db.res->next()) {
                    //display item list first
                    system("cls");
                    item.displayAllItemList();
                    cout << endl << endl;

                    //Select Item to delete
                    while (true) {
                        cout << YELLOW << "Enter Item ID to delete (or enter 0 to return user menu): " << RESET;
                        cin >> itemid;

                        // Check if input is numeric
                        if (cin.fail() || cin.peek() != '\n') {
                            cin.clear(); // Clear error flag
                            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                            cout << RED << "Invalid input! Please enter a valid numeric Item ID." << RESET << endl;
                            continue;
                        }

                        if (itemid == 0) {
                            system("cls");
                            cout << BYELLOW << "Returning to menu..." << RESET << endl;
                            break; // Exit the delete process entirely
                        }

                        // Check if Item ID exists in the database for this user
                        db.prepareStatement("SELECT * FROM item WHERE Item_ID = ? AND Item_Status = 'Available'");
                        db.stmt->setInt(1, itemid);
                        db.QueryResult();

                        if (db.res->next()) {
                            char choice;
                            while (true) { // Start an infinite loop to keep asking for input
                                cout << RED << endl << "Are you sure you want to delete this item? (Y/N): " << RESET;
                                cin >> choice;  // Read only the first character
                                if (cin.peek() != '\n') {
                                    // If there are extra characters, ignore the rest and consider it invalid
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    cout << RED << "Invalid choice! Please enter only 'Y' or 'N'." << RESET << endl;
                                    continue;  // Prompt again
                                }

                                // Check if the input is valid (Y/y or N/n)
                                if (choice == 'Y' || choice == 'y') {
                                    item.adminRemoveItem(itemid);
                                    system("cls");
                                    cout << CYAN << "( Item deleted successfully! )" << RESET << endl;
                                    break; // Exit the loop if the user confirms the addition
                                }
                                else if (choice == 'N' || choice == 'n') {
                                    system("cls");
                                    cout << YELLOW << "Cancelled. Returning to menu..." << RESET << endl;
                                    break; // Exit the loop if the user cancels the action
                                }
                                else {
                                    // Invalid choice, print error and prompt again
                                    cout << RED << "Invalid choice! Please enter 'Y' to confirm or 'N' to cancel." << RESET << endl;
                                }
                            }
                            // Valid Item ID found
                            break;
                        }
                        else {
                            cout << RED << "Item ID does not exist or be Sold. Please try again." << RESET << endl;
                        }
                    }


                }

                else {
                    system("cls");
                    cout << "You don't have any items!" << endl;
                }


            }
            else {
                cout << "No items available!" << endl;

            }
            }

        //View Report
        else if (adminChoice == 4) {
            Item item;
            int choice;
            int insertChoice;
            while (true) {
                system("cls");
                cout << GREEN << "========================================" << RESET << endl;
                cout << CYAN << "               VIEW REPORT               " << RESET << endl;
                cout << GREEN << "========================================" << RESET << endl << endl;
                cout << BYELLOW << "Select a Report to Generate:" << RESET << endl;
                cout << BWHITE << "1. Profit Fee Report (Monthly)" << endl;
                cout << "2. Profit Fee Report (Annual)" << endl;
                cout << "3. Transaction Report (Annual)" << endl;
                cout << "0. Return to menu" << endl << endl << RESET;
                REPORTCHOICE:
                cout << BYELLOW << "Enter your choice: " << RESET;
                cin >> choice;

                // Check if input is numeric
                if (cin.fail() || cin.peek() != '\n') {
                    cin.clear(); // Clear error flag
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                    cout << RED << "Invalid input! Please try again!" << RESET << endl;
                    goto REPORTCHOICE;
                }

                switch (choice) {
                case 1:
                    item.generateMonthlyProfitFeeReport();
                    cout << endl << endl << endl << BYELLOW << "Press any key to return to menu..." << RESET;
                    _getch();  // Wait for a key press
                    system("cls");
                    cout << BYELLOW << "Returning to menu..." << RESET << endl;
                    continue;
                case 2:
                    item.generateAnnualProfitFeeReport();
                    cout << endl << endl << endl << BYELLOW << "Press any key to return to menu..." << RESET;
                    _getch();  // Wait for a key press
                    system("cls");
                    cout << BYELLOW << "Returning to menu..." << RESET << endl;

                    continue;

                case 3:
                    item.generateAnnualTransactionReport();
                    cout << endl << endl << endl << BYELLOW << "Press any key to return to menu..." << RESET;
                    _getch();  // Wait for a key press
                    system("cls");
                    cout << BYELLOW << "Returning to menu..." << RESET << endl;

                    continue;

                case 0:
                    system("cls");
                    cout << BYELLOW << "Returning to menu..." << RESET << endl;
                    break;                    


                default:
                    cout << RED << "Invalid choice! Please try again!" << RESET << endl;
                    goto REPORTCHOICE;
                }                    
                goto AdminMenu;

            }
        }

        //Logout
        else if (adminChoice == 0) {
            system("cls");
            cout << YELLOW << "You have been logged out. Returning to main menu..." << RESET << endl;
            break;  // Break the loop to return to the main menu
        }
        
        else {
            system("cls");
            cout << RED << "Invalid choice! Please try again!" << RESET << endl;
        }
    }
}
