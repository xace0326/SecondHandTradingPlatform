#include <iostream>
#include <iomanip>
#include <mysql/jdbc.h>
#include "db.h"
#include "item.h"
#include "menu.h"
#include <conio.h>
#include <string>
#pragma warning(disable : 4996)
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


void Item::addItem(int userid, string itemName, string itemDescription, double itemPrice, int category) {

    DBConnection db;
    // Step 1: Get the highest item_ID from the database
    db.prepareStatement("SELECT MAX(Item_ID) FROM item");
    db.QueryResult();

    int newItemID = 30001;  // Start User_ID from 10001

    // If there are existing users, we get the max User_ID and increment it
    if (db.res->next()) {
        int maxItemID = db.res->getInt(1);
        if (maxItemID > 0) {
            newItemID = maxItemID + 1;  // Increment the highest User_ID by 1
        }
    }

    // Auto fill in status
    time_t now = time(NULL);
    struct tm date = *localtime(&now);
    char currentDate[80];
    strftime(currentDate, sizeof(currentDate), "%Y-%m-%d", &date); // Get current date in YYYY-MM-DD format

    db.prepareStatement("INSERT INTO item (Item_ID, User_ID, Item_Name, Item_Description, Item_Price, Category_ID, Item_Post_Date) VALUES (?, ?, ?, ?, ?, ?, ?)");
    db.stmt->setInt(1, newItemID); // Assume userID is set
    db.stmt->setInt(2, userid);
    db.stmt->setString(3, itemName);
    db.stmt->setString(4, itemDescription);
    db.stmt->setDouble(5, itemPrice);
    db.stmt->setInt(6, category);
    db.stmt->setString(7, currentDate);

    db.stmt->executeUpdate();

}

void Item::removeItem(int itemID, int userID) {
    DBConnection db;

    // Step 1: Check if the item exists and belongs to the user
    db.prepareStatement("SELECT COUNT(*) FROM item WHERE Item_ID = ? AND User_ID = ?");
    db.stmt->setInt(1, itemID);
    db.stmt->setInt(2, userID);

    db.QueryResult();

    if (db.res->next() && db.res->getInt(1) > 0) {
        // Delete the item
        db.prepareStatement("DELETE FROM item WHERE Item_ID = ?"); 
        db.stmt->setInt(1, itemID);

        try {
            db.stmt->executeUpdate();
            cout << "Item removed successfully!" << endl;
        }
        catch (sql::SQLException& e) {
            cout << "Error removing item: " << e.what() << endl;
        }
    }
    else {
        system("cls");
        cout << "Item not found or you do not have permission to remove it." << endl;
    }
}

void Item::adminRemoveItem(int itemID) {
    DBConnection db;

    // Step 1: Check if the item exists and belongs to the user
    db.prepareStatement("SELECT COUNT(*) FROM item WHERE Item_ID = ?");
    db.stmt->setInt(1, itemID);

    db.QueryResult();

    if (db.res->next() && db.res->getInt(1) > 0) {
        // Delete the item
        db.prepareStatement("DELETE FROM item WHERE Item_ID = ?");
        db.stmt->setInt(1, itemID);

        try {
            db.stmt->executeUpdate();
            cout << "Item removed successfully!" << endl;
        }
        catch (sql::SQLException& e) {
            cout << "Error removing item: " << e.what() << endl;
        }
    }
    else {
        cout << "Item not found!" << endl;
    }
}

void Item::displayUserAvailableItemList(int userID) {
    DBConnection db;

    db.prepareStatement("SELECT Item_ID, User_ID, Item_Name, Item_Description, Item_Price, Category_Name, Item_Post_Date, Item_Status FROM item JOIN category USING (Category_ID) WHERE User_ID= ? AND Item_Status = 'Available'");
    db.stmt->setInt(1, userID);
    db.QueryResult();

    if (db.res->next()) {
        // Display headers
        cout << BYELLOW << left << setw(5) << "ID" << " | "
            << setw(22) << "Item Name" << " | "
            << setw(40) << "Description" << " | "
            << setw(15) << "Price (RM)" << " | "
            << setw(15) << "Category" << " | "
            << setw(11) << "Post Date" << " | "
            << setw(10) << "Status" << " | " << RESET << endl;

        cout << YELLOW <<  string(138, '-') << RESET << endl;

        do {
            // Truncate Item Name and Item Description if they are too long
            string itemName = db.res->getString("Item_Name");
            string itemDescription = db.res->getString("Item_Description");

            if (itemName.length() > 18) {
                itemName = itemName.substr(0, 18) + "...";
            }

            if (itemDescription.length() > 35) {
                itemDescription = itemDescription.substr(0, 35) + "...";
            }

            // Display each item with its corresponding details
            cout << left << setw(5) << db.res->getInt("Item_ID") << " | "
                << BCYAN << setw(22) << itemName << RESET << " | "
                << setw(40) << itemDescription << " | "
                << setw(15) << fixed << setprecision(2) << db.res->getDouble("Item_Price") << " | "
                << setw(15) << db.res->getString("Category_Name") << " | "
                << setw(11) << db.res->getString("Item_Post_Date") << " | "
                << setw(10) << db.res->getString("Item_Status") << " | " << endl;


        } while (db.res->next());
        cout << string(138, '-') << endl; // Separating line between items

    }
}

void Item::displayUserSoldItemList(int userID) {
    DBConnection db;

    db.prepareStatement("SELECT item.Item_ID, item.User_ID, item.Item_Name, item.Item_Description, item.Item_Price, category.Category_Name, item.Item_Post_Date, item.Item_Status,transaction.Transaction_ID, transaction.Transaction_Status, transaction.Transaction_Date, transaction.Amount, transaction.Fee, transaction.Transaction_Status FROM item JOIN category ON category.Category_ID = item.Category_ID JOIN transaction ON item.Item_ID = transaction.Item_ID WHERE item.User_ID = ? AND item.Item_Status = 'Sold'");
    db.stmt->setInt(1, userID);
    db.QueryResult();

    if (db.res->next()) {
        // Display headers
        cout << BYELLOW << left << setw(5) << "ID" << " | "
            << setw(22) << "Item Name" << " | "
            << setw(40) << "Description" << " | "
            << setw(15) << "Price (RM)" << " | "
            << setw(11) << "Category" << " | "
            << setw(11) << "Post Date" << " | "
            << setw(10) << "Sold Date" << " | "
            << setw(10) << "Status" << " | " << RESET << endl;

        cout << YELLOW << string(147, '-') << RESET << endl;

        do {
            // Truncate Item Name and Item Description if they are too long
            string itemName = db.res->getString("Item_Name");
            string itemDescription = db.res->getString("Item_Description");

            if (itemName.length() > 18) {
                itemName = itemName.substr(0, 18) + "...";
            }

            if (itemDescription.length() > 35) {
                itemDescription = itemDescription.substr(0, 35) + "...";
            }

            // Display each item with its corresponding details
            cout << left << setw(5) << db.res->getInt("Transaction_ID") << " | "
                << BCYAN << setw(22) << itemName << RESET << " | "
                << setw(40) << itemDescription << " | "
                << setw(15) << fixed << setprecision(2) << db.res->getDouble("Item_Price") << " | "
                << setw(11) << db.res->getString("Category_Name") << " | "
                << setw(11) << db.res->getString("Item_Post_Date") << " | "
                << setw(10) << db.res->getString("Transaction_Date") << " | "
                << setw(10) << db.res->getString("Transaction_Status") << " | " << endl;


        } while (db.res->next());
        cout << string(147, '-') << endl; // Separating line between items
    }
}

void Item::displayUserPurchaseItemList(int userID) {
    DBConnection db;

    // Query to fetch items purchased by the user
    db.prepareStatement(
        "SELECT item.Item_ID, item.Item_Name, item.Item_Description, item.Item_Price, "
        "category.Category_Name, item.Item_Post_Date, item.Item_Status, transaction.Transaction_Date, "
        "transaction.Amount, transaction.Fee, transaction.Transaction_Status, transaction.Transaction_ID "
        "FROM item "
        "JOIN category ON category.Category_ID = item.Category_ID "
        "JOIN transaction ON item.Item_ID = transaction.Item_ID "
        "WHERE transaction.Buyer_ID = ? AND item.Item_Status = 'Sold' AND transaction.Transaction_Status = 'Completed'"
        "ORDER BY transaction.Transaction_Date ASC");
    db.stmt->setInt(1, userID); // Assuming `userID` is the logged-in user's ID
    db.QueryResult();

    if (db.res->next()) {
        // Display headers
        cout << BYELLOW << left << setw(5) << "ID" << " | "
            << setw(22) << "Item Name" << " | "
            << setw(40) << "Description" << " | "
            << setw(15) << "Price (RM)" << " | "
            << setw(11) << "Category" << " | "
            << setw(11) << "Post Date" << " | "
            << setw(15) << "Purchase Date" << " | " << RESET << endl;

        cout << YELLOW << string(139, '-') << RESET << endl;

        do {
            // Truncate Item Name and Item Description if they are too long
            string itemName = db.res->getString("Item_Name");
            string itemDescription = db.res->getString("Item_Description");

            if (itemName.length() > 18) {
                itemName = itemName.substr(0, 18) + "...";
            }

            if (itemDescription.length() > 35) {
                itemDescription = itemDescription.substr(0, 35) + "...";
            }

            // Display each item with its corresponding details
            cout << left << setw(5) << db.res->getInt("Transaction_ID") << " | "
                << BCYAN << setw(22) << itemName << RESET << " | "
                << setw(40) << itemDescription << " | "
                << setw(15) << fixed << setprecision(2) << db.res->getDouble("Item_Price") << " | "
                << setw(11) << db.res->getString("Category_Name") << " | "
                << setw(11) << db.res->getString("Item_Post_Date") << " | "
                << setw(15) << db.res->getString("Transaction_Date") << " | " << endl;

        } while (db.res->next());

        cout << string(139, '-') << endl; // Separating line between items
    }
    else {
        system("cls");
        cout << RED << "You have not purchased any items yet!" << RESET << endl;
    }

}

void Item::displayAllItemList() {
    DBConnection db;

    db.prepareStatement("SELECT Item_ID, User_ID, Item_Name, Item_Description, Item_Price, Category_Name, Item_Post_Date, Item_Status FROM item JOIN category USING (Category_ID) WHERE Item_Status = 'Available' ORDER BY Item_ID ASC");
    db.QueryResult();

    //Display the results
    cout << BYELLOW << left << setw(5) << "ID" <<" | "
        << setw(22) << "Item Name" << " | "
        << setw(40) << "Description" << " | "
        << setw(15) << "Price (RM)" << " | "
        << setw(15) << "Category" << " | "
        << setw(11) << "Post Date" << " | " << RESET << endl;

    cout << YELLOW << string(125, '-') << RESET << endl;

    while (db.res->next()) {
        // Truncate Item Name and Item Description if they are too long
        string itemName = db.res->getString("Item_Name");
        string itemDescription = db.res->getString("Item_Description");

        if (itemName.length() > 18) {
            itemName = itemName.substr(0, 18) + "...";
        }

        if (itemDescription.length() > 35) {
            itemDescription = itemDescription.substr(0, 35) + "...";
        }


        cout << left << setw(5) << db.res->getInt("Item_ID") << " | "
            << BCYAN << setw(22) << itemName << RESET << " | "
            << setw(40) << itemDescription << " | "
            << setw(15) << fixed << setprecision(2) << db.res->getDouble("Item_Price") << " | "
            << setw(15) << db.res->getString("Category_Name") << " | "
            << setw(11) << db.res->getString("Item_Post_Date") << " | " << endl;


    }  
    cout << string(125, '-') << endl; // Separating line between items

} // All Available Item List

void Item::displaySoldItemList() {
    DBConnection db;

    db.prepareStatement(
        "SELECT item.Item_ID, item.User_ID, item.Item_Name, item.Item_Description, item.Item_Price, "
        "category.Category_Name, item.Item_Post_Date, item.Item_Status "
        "FROM item "
        "JOIN category USING (Category_ID) "
        "JOIN transaction ON item.Item_ID = transaction.Item_ID "
        "WHERE item.Item_Status = 'Sold' AND transaction.Transaction_Status = 'Completed' "
        "ORDER BY item.Item_ID ASC"
    );
    db.QueryResult();

    //Display the results
    cout << BYELLOW << left << setw(5) << "ID" << " | "
        << setw(22) << "Item Name" << " | "
        << setw(40) << "Description" << " | "
        << setw(15) << "Price (RM)" << " | "
        << setw(15) << "Category" << " | "
        << setw(11) << "Post Date" << " | " << RESET << endl;

    cout << YELLOW << string(125, '-') << RESET << endl;

    while (db.res->next()) {
        // Truncate Item Name and Item Description if they are too long
        string itemName = db.res->getString("Item_Name");
        string itemDescription = db.res->getString("Item_Description");

        if (itemName.length() > 18) {
            itemName = itemName.substr(0, 18) + "...";
        }

        if (itemDescription.length() > 35) {
            itemDescription = itemDescription.substr(0, 35) + "...";
        }


        cout << left << setw(5) << db.res->getInt("Item_ID") << " | "
            << BCYAN << setw(22) << itemName << RESET << " | "
            << setw(40) << itemDescription << " | "
            << setw(15) << fixed << setprecision(2) << db.res->getDouble("Item_Price") << " | "
            << setw(15) << db.res->getString("Category_Name") << " | "
            << setw(11) << db.res->getString("Item_Post_Date") << " | " << endl;


    }
    cout << string(125, '-') << endl; // Separating line between items

} // All Sold Item List

void Item::displayItemDetails(int itemID, int userID) {
    DBConnection db;
    Menu menu;
    db.prepareStatement("SELECT Item_ID, User_ID, Item_Name, Item_Description, Item_Price, Category_Name, Item_Post_Date, Item_Status FROM item JOIN category USING (Category_ID) WHERE Item_ID= ? AND User_ID = ?");
    db.stmt->setInt(1, itemID);
    db.stmt->setInt(2, userID);
    db.QueryResult();
    if (db.res->next()) {
        string itemName = db.res->getString("Item_Name");
        double itemPrice = db.res->getDouble("Item_Price");

        system("cls");
        cout << GREEN << "========================================" << RESET << endl;
        cout << CYAN << "              ITEM DETAILS               " << RESET << endl;
        cout << GREEN << "========================================" << RESET << endl << endl;
        cout << YELLOW << string(150, '*') << RESET << endl;
        cout << BWHITE << "Item ID: " << itemID << endl;
        cout << "Item Name: " << itemName << endl;
        cout << "Item Description: " << db.res->getString("Item_Description") << endl;
        cout << "Category: " << db.res->getString("Category_Name") << endl;
        cout << RED << "Item Price: RM" << itemPrice << RESET <<endl;
        cout << BWHITE << "Item Post Date: " << db.res->getString("Item_Post_Date") << RESET << endl;
        cout << RESET << GREEN << "Item Status: " << db.res->getString("Item_Status") << RESET << endl;
        cout << YELLOW << string(150, '*') << RESET << endl<<endl;
    }
    else {
        system("cls");
        cout << RED << "Item not found in your inventory!" << RESET << endl;
    }
}

void Item::AdminDisplayItemDetails(int itemID) {
    DBConnection db;
    Menu menu;
    db.prepareStatement("SELECT item.Item_ID, item.User_ID, item.Item_Name, item.Item_Description, item.Item_Price, category.Category_Name, item.Item_Post_Date, item.Item_Status, user.Name, user.Email, user.Phone FROM item JOIN category ON category.Category_ID = item.Category_ID JOIN user ON item.User_ID = user.User_ID WHERE item.Item_ID = ? AND item.Item_Status = 'Available'");
    db.stmt->setInt(1, itemID);
    db.QueryResult();
    if (db.res->next()) {
        string itemName = db.res->getString("Item_Name");
        double itemPrice = db.res->getDouble("Item_Price");

        system("cls");
        cout << GREEN << "========================================" << RESET << endl;
        cout << CYAN << "              ITEM DETAILS               " << RESET << endl;
        cout << GREEN << "========================================" << RESET << endl << endl;
        cout << YELLOW << string(150, '*') << RESET << endl;
        cout << BWHITE << "Item ID: " << itemID << endl;
        cout << "Item Name: " << itemName << endl;
        cout << "Item Description: " << db.res->getString("Item_Description") << endl;
        cout << "Category: " << db.res->getString("Category_Name") << endl;
        cout << "Item Post Date: " << db.res->getString("Item_Post_Date") << RESET << endl;        
        cout << RED << "Item Price: RM" << itemPrice << RESET << endl;

        cout << RESET << GREEN << "Item Status: " << db.res->getString("Item_Status") << RESET << endl;
        cout << BCYAN << string(50, '-') << RESET << endl;
        cout << BYELLOW << "Seller's Information: " << RESET << endl;
		cout << BWHITE << "Seller ID: " << RESET << BCYAN << db.res->getInt("User_ID") << RESET << endl;
        cout << BWHITE << "Name: " << RESET << BCYAN << db.res->getString("Name") << RESET << endl;
        cout << BWHITE << "Seller's Email: " << RESET << BCYAN << db.res->getString("Email") << RESET << endl;
        cout << BWHITE << "Seller's Contact Number: " << RESET << BCYAN << db.res->getString("Phone") << RESET << endl;
        cout << BCYAN << string(50, '-') << RESET << endl;

        cout << YELLOW << string(150, '*') << RESET << endl << endl;
    }
    else {
        system("cls");
        cout << RED << "Item not found or SOLD!" << RESET << endl;
    }
}

void Item::displaySoldItemDetails(int itemID, int userID) {
    DBConnection db;
    Menu menu;
    db.prepareStatement("SELECT item.Item_ID, item.User_ID, item.Item_Name, item.Item_Description, item.Item_Price, category.Category_Name, item.Item_Post_Date, item.Item_Status,transaction.Transaction_ID, transaction.Buyer_Address, transaction.Transaction_Status,transaction.Transaction_Date, transaction.Amount, transaction.Fee, user.Name, user.Email, user.Phone FROM item JOIN category ON category.Category_ID = item.Category_ID JOIN transaction ON item.Item_ID = transaction.Item_ID  JOIN user ON transaction.Buyer_ID = user.User_ID WHERE transaction.Transaction_ID = ? AND item.User_ID = ? AND item.Item_Status = 'Sold'");
    db.stmt->setInt(1, itemID);
    db.stmt->setInt(2, userID);
    db.QueryResult();
    if (db.res->next()) {
        string itemName = db.res->getString("Item_Name");
        double itemPrice = db.res->getDouble("Item_Price");
        string buyerAddress = db.res->getString("Buyer_Address");  // Get Buyer Address

        system("cls");
        cout << GREEN << "========================================" << RESET << endl;
        cout << CYAN << "            ITEM SOLD DETAILS             " << RESET << endl;
        cout << GREEN << "========================================" << RESET << endl << endl;
        cout << YELLOW << string(150, '*') << RESET << endl;
        cout << BWHITE << "Transaction ID: " << RESET << BCYAN << db.res->getInt("Transaction_ID") << RESET << endl;
        cout << BWHITE << "Item ID: " << RESET << BCYAN << db.res->getInt("Item_ID") << RESET << endl;
        cout << BWHITE << "Item Name: " << itemName << endl;
        cout << BWHITE << "Item Description: " << db.res->getString("Item_Description") << endl;
        cout << BWHITE << "Category: " << db.res->getString("Category_Name") << endl;        
        cout << BWHITE << "Item Price: RM" << itemPrice << RESET << endl;
        cout << BWHITE << "Item Post Date: " << db.res->getString("Item_Post_Date") << endl;
        cout << BWHITE << "Item Sold Date: " << db.res->getString("Transaction_Date") << RESET << endl;
        cout << RED << "Item Status: " << db.res->getString("Item_Status") << RESET << endl;         
        cout << GREEN << "Earning: RM" << itemPrice << RESET << endl;

        cout << BCYAN << string(60, '-') << RESET << endl;
        cout << BYELLOW  << "The item was purchased by " << RESET << BCYAN << db.res->getString("Name") << RESET << endl;
        cout << BYELLOW << "Buyer's Information: " << RESET << endl;
        cout << BWHITE << " -Name: " << RESET << BCYAN << db.res->getString("Name") << RESET << endl;
        cout << BWHITE << " -Buyer's Email: "<< RESET << BCYAN << db.res->getString("Email") << RESET << endl;
        cout << BWHITE << " -Buyer's Contact Number: " << RESET << BCYAN << db.res->getString("Phone") << RESET << endl;
        cout << BWHITE << " -Delivery Address: " << RESET << BCYAN << buyerAddress << RESET << endl;
        cout << BCYAN << string(60, '-') << RESET << endl;
        cout << RESET << BYELLOW << "Transaction Status: " << BCYAN << db.res->getString("Transaction_Status") << RESET << endl;
        cout << BCYAN << string(60, '-') << RESET << endl;
        cout << YELLOW << string(150, '*') << RESET << endl << endl;
    }
    else {
        system("cls");
        cout << RED << "Item not found in your inventory!" << RESET << endl;
    }
}

void Item::AdminDisplaySoldItemDetails(int itemID) {
    DBConnection db;
    Menu menu;
    db.prepareStatement(
        "SELECT item.Item_ID, item.User_ID AS Seller_ID, transaction.Buyer_ID AS Buyer_ID, item.Item_Name, item.Item_Description, "
        "item.Item_Price, category.Category_Name, item.Item_Post_Date, item.Item_Status, "
        "transaction.Transaction_Date, transaction.Amount, transaction.Fee, transaction.Transaction_Status,transaction.Buyer_Address,transaction.Transaction_ID,"
        "seller.Name AS Seller_Name, seller.Email AS Seller_Email, seller.Phone AS Seller_Phone, "
        "buyer.Name AS Buyer_Name, buyer.Email AS Buyer_Email, buyer.Phone AS Buyer_Phone "
        "FROM item "
        "JOIN category ON category.Category_ID = item.Category_ID "
        "JOIN transaction ON item.Item_ID = transaction.Item_ID "
        "JOIN user AS seller ON item.User_ID = seller.User_ID "
        "JOIN user AS buyer ON transaction.Buyer_ID = buyer.User_ID "
        "WHERE item.Item_ID = ? AND item.Item_Status = 'Sold' AND transaction.Transaction_Status = 'Completed'"
    );

    // Set the itemID parameter
    db.stmt->setInt(1, itemID);
    db.QueryResult();

    // Check if a result is returned
    if (db.res->next()) {
        // Retrieve seller and buyer information
        string sellerName = db.res->getString("Seller_Name");
        string sellerEmail = db.res->getString("Seller_Email");
        string sellerPhone = db.res->getString("Seller_Phone");

        string buyerName = db.res->getString("Buyer_Name");
        string buyerEmail = db.res->getString("Buyer_Email");
        string buyerPhone = db.res->getString("Buyer_Phone");
        string buyerAddress = db.res->getString("Buyer_Address");  // Get Buyer Address

        //Item details price
        double itemPrice = db.res->getDouble("Item_Price");
        double fee = db.res->getDouble("Fee");
        double totalCost = itemPrice + fee;

        // Display item details
        system("cls");
        cout << GREEN << "========================================" << RESET << endl;
        cout << CYAN << "            ITEM SOLD DETAILS             " << RESET << endl;
        cout << GREEN << "========================================" << RESET << endl << endl;
        cout << YELLOW << string(150, '*') << RESET << endl;
        cout << BWHITE << "Transaction ID:" << RESET << BCYAN << db.res->getInt("Transaction_ID") << RESET << endl;
        cout << BWHITE << "Item ID: " << RESET << BCYAN << itemID << RESET << endl;
        cout << BWHITE << "Item Name: " << RESET << BCYAN << db.res->getString("Item_Name") << RESET << endl;
        cout << BWHITE << "Item Description: " << db.res->getString("Item_Description") << endl;
        cout << BWHITE << "Category: "  << db.res->getString("Category_Name") << endl;
        cout << BWHITE << "Item Post Date: "  << db.res->getString("Item_Post_Date") << endl;
        cout << BWHITE << "Item Sold Date: "  << db.res->getString("Transaction_Date") << endl;
        cout << RED << "Item Status: "  << db.res->getString("Item_Status") << RESET <<endl;

        cout << BCYAN << string(60, '-') << RESET << endl;
        cout << RED << "Item Price: RM" << fixed << setprecision(2) << itemPrice << RESET << endl;
        cout << RED << "3% Platform Fee: RM" << fee << RESET << endl;
        cout << GREEN << "Total Cost: RM" << totalCost << RESET << endl;

        // Display seller information
        cout << BCYAN << string(60, '-') << RESET << endl;
        cout << BYELLOW << "Seller's Information: " << RESET << endl;
        cout << BWHITE << "Seller ID: " << RESET << BCYAN << db.res->getInt("Seller_ID") << RESET << endl;
        cout << BWHITE << "Name: " << RESET << BCYAN << sellerName << RESET << endl;
        cout << BWHITE << "Seller's Email: " << RESET << BCYAN << sellerEmail << RESET << endl;
        cout << BWHITE << "Seller's Contact Number: " << RESET << BCYAN << sellerPhone << RESET << endl;

        // Display buyer information
        cout << BCYAN << string(60, '-') << RESET << endl;
        cout << BYELLOW << "Buyer's Information: " << RESET << endl;
        cout << BWHITE << "Buyer ID: " << RESET << BCYAN << db.res->getInt("Buyer_ID") << RESET << endl;
        cout << BWHITE << "Name: " << RESET << BCYAN << buyerName << RESET << endl;
        cout << BWHITE << "Buyer's Email: " << RESET << BCYAN << buyerEmail << RESET << endl;
        cout << BWHITE << "Buyer's Contact Number: " << RESET << BCYAN << buyerPhone << RESET << endl;
        cout << BWHITE << "Delivery Address: " << RESET << BCYAN << buyerAddress << RESET << endl;

        cout << BCYAN << string(60, '-') << RESET << endl;
        cout << YELLOW << string(150, '*') << RESET << endl << endl;
    }
    else {
        cout << RED << "No sold item details found for the given Item ID!" << RESET << endl;
    }
}

void Item::displayPurchaseItemDetails(int itemID, int userID) {
    DBConnection db;
    Menu menu;

    // Query to fetch items purchased by the user
    db.prepareStatement("SELECT item.Item_ID, item.User_ID, item.Item_Name, item.Item_Description, item.Item_Price, category.Category_Name, item.Item_Post_Date, item.Item_Status,transaction.Transaction_ID, transaction.Buyer_Address, transaction.Transaction_Date, transaction.Amount, transaction.Fee, transaction.Transaction_Status, user.Name, user.Email, user.Phone "
        "FROM item "
        "JOIN category ON category.Category_ID = item.Category_ID "
        "JOIN transaction ON item.Item_ID = transaction.Item_ID "
        "JOIN user ON transaction.Seller_ID = user.User_ID "
        "WHERE transaction.Transaction_ID = ? AND transaction.Buyer_ID = ? AND item.Item_Status = 'Sold' AND transaction.Transaction_Status = 'Completed'");
    db.stmt->setInt(1, itemID);
    db.stmt->setInt(2, userID); // Assuming `userID` represents the logged-in user's ID
    db.QueryResult();

    if (db.res->next()) {
        system("cls");
        cout << GREEN << "========================================" << RESET << endl;
        cout << CYAN << "           MY PURCHASE DETAILS          " << RESET << endl;
        cout << GREEN << "========================================" << RESET << endl << endl;

        do {
            string itemName = db.res->getString("Item_Name");
            double itemPrice = db.res->getDouble("Item_Price");
            double fee = db.res->getDouble("Fee");
            double totalCost = itemPrice + fee;
            string buyerAddress = db.res->getString("Buyer_Address");  // Get Buyer Address

            cout << YELLOW << string(150, '*') << RESET << endl;
            cout << BWHITE << "Transaction ID: " << BCYAN << db.res->getInt("Transaction_ID") << endl;
            cout << BWHITE << "Item ID: " << BCYAN << db.res->getInt("Item_ID") << RESET << endl;
            cout << BWHITE << "Item Name: " << itemName << endl;
            cout << "Item Description: " << db.res->getString("Item_Description") << endl;
            cout << "Category: " << db.res->getString("Category_Name") << endl;
            cout << "Item Post Date: " << db.res->getString("Item_Post_Date") << endl;
            cout << "Purchase Date: " << db.res->getString("Transaction_Date") << endl;
            cout << "Delivery Address: " << RESET << BCYAN << buyerAddress << RESET << endl;

            cout << BCYAN << string(60, '-') << RESET << endl;
            cout << BYELLOW << "Seller's Information: " << RESET << endl;
            cout << BWHITE << "Name: " << RESET << BCYAN << db.res->getString("Name") << RESET << endl;
            cout << BWHITE << "Seller's Email: " << RESET << BCYAN << db.res->getString("Email") << RESET << endl;
            cout << BWHITE << "Seller's Contact Number: " << RESET << BCYAN << db.res->getString("Phone") << RESET << endl;

            cout << BCYAN << string(60, '-') << RESET << endl;
            cout << RED << "Item Price: RM" << fixed << setprecision(2) << itemPrice << RESET << endl;
            cout << RED << "3% Platform Fee: RM" << fee << RESET << endl;
            cout << GREEN << "Total Cost: RM" << totalCost << RESET << endl;
            cout << BCYAN << string(60, '-') << RESET << endl;
            cout << RESET << BYELLOW << "Transaction Status: " << BCYAN << db.res->getString("Transaction_Status") << RESET << endl;
            cout << BCYAN << string(60, '-') << RESET << endl;
            cout << YELLOW << string(150, '*') << RESET << endl << endl;
        } while (db.res->next());
    }
    else {
        system("cls");
        cout << RED << "You have not purchased any items!" << RESET << endl;
    }

}

void Item::buyItem(int buyerID, int itemID) {
    DBConnection db;
    Menu menu;

    // Update query to include buyer's address
    db.prepareStatement(
        "SELECT item.Item_ID, item.User_ID AS Seller_ID, item.Item_Name, item.Item_Description, item.Item_Price, "
        "category.Category_Name, item.Item_Post_Date, item.Item_Status, seller.Name AS Seller_Name, seller.Email AS Seller_Email, "
        "seller.Phone AS Seller_Phone, buyer.Address AS Buyer_Address "
        "FROM item "
        "JOIN category ON item.Category_ID = category.Category_ID "
        "JOIN user AS seller ON item.User_ID = seller.User_ID "
        "JOIN user AS buyer ON buyer.User_ID = ? "
        "WHERE item.Item_ID = ?"
    );

    db.stmt->setInt(1, buyerID);  // Buyer ID passed to the query
    db.stmt->setInt(2, itemID);   // Item ID passed to the query
    db.QueryResult();

    if (db.res->next()) {
        int sellerID = db.res->getInt("Seller_ID");
        string itemName = db.res->getString("Item_Name");
        string itemDescription = db.res->getString("Item_Description");
        double itemPrice = db.res->getDouble("Item_Price");
        string sellerName = db.res->getString("Seller_Name");
        string sellerEmail = db.res->getString("Seller_Email");
        string sellerPhone = db.res->getString("Seller_Phone");
        string buyerAddress = db.res->getString("Buyer_Address");
        double fee = itemPrice * 0.03;
        double totalPrice = fee + itemPrice;

        // Step 2: Check if the buyer is trying to buy their own item
        if (buyerID == sellerID) {
            system("cls");
            cout << RED << "You cannot buy your own item!" << RESET << endl;
            menu.userMenu(buyerID); // Call the user menu function
            return;
        }

        system("cls");
        cout << GREEN << "========================================" << RESET << endl;
        cout << CYAN << "                 BUY ITEM               " << RESET << endl;
        cout << GREEN << "========================================" << RESET << endl << endl;
        cout << YELLOW << string(150, '*') << RESET << endl;
        cout << BWHITE << "Item ID: " << itemID << endl;
        cout << "Item Name: " << itemName << endl;
        cout << "Item Description: " << itemDescription << endl;
        cout << "Category: " << db.res->getString("Category_Name") << RESET << endl;
        cout << BCYAN << string(50, '-') << RESET << endl;

        // Display Seller's Information
        cout << BYELLOW << "Seller's Information: " << RESET << endl;
        cout << BWHITE << "Name: " << RESET << BCYAN << sellerName << RESET << endl;
        cout << BWHITE << "Seller's Email: " << RESET << BCYAN << sellerEmail << RESET << endl;
        cout << BWHITE << "Seller's Contact Number: " << RESET << BCYAN << sellerPhone << RESET << endl;
        cout << BCYAN << string(50, '-') << RESET << endl;

        // Display Item Price and Fee Information
        cout << RED << "Item Price: RM" << itemPrice << RESET << endl;
        cout << RED << "3% Platform Fee: RM" << fee << endl << RESET;
        cout << GREEN << "Total Price: RM" << totalPrice << RESET << endl;
        cout << BCYAN << string(50, '-') << RESET << endl;
        cout << YELLOW << string(150, '*') << RESET << endl << endl;

        // Display the Buyer’s Address

        char choice;
        while (true) {  // Start an infinite loop to keep asking for input
            cout << CYAN << "Are you sure you want to buy this item? (Y/N): " << RESET;
            cin >> choice;
            if (cin.peek() != '\n') {
                // If there are extra characters, ignore the rest and consider it invalid
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << RED << "Invalid choice! Please enter only 'Y' or 'N'." << RESET << endl;
                continue;  // Prompt again
            }

            // Check for valid input
            if (choice == 'Y' || choice == 'y') {
                int choiceMethod;
                string paymentMethod;

                system("cls");
                cout << GREEN << "========================================" << RESET << endl;
                cout << CYAN << "                 CHECKOUT               " << RESET << endl;
                cout << GREEN << "========================================" << RESET << endl << endl;

                cout << YELLOW << "Your Address: " << RESET << BCYAN << buyerAddress << RESET << endl;
                // Display Payment Method Options
                cout << BYELLOW << "Choose Payment Method:" << RESET << endl;
                cout << BWHITE << "      1. Credit Card" << RESET << endl;
                cout << BWHITE << "      2. TNG" << RESET << endl;
                PAYMENTCHOICE:
                cout << YELLOW << "Enter your choice (1 or 2): " << RESET;
                cin >> choiceMethod;
                if (cin.fail() || cin.peek() != '\n') {
                    cin.clear(); // Clear error flag
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                    cout << RED << "Invalid input! Please try again!" << RESET << endl;
                    goto PAYMENTCHOICE;
                }

                // Validate Payment Method
                if (choiceMethod == 1) {
                    cout << GREEN << "You selected Credit Card." << RESET << endl;
                    paymentMethod = "Credit Card";
                }
                else if (choiceMethod == 2) {
                    cout << GREEN << "You selected TNG." << RESET << endl;
                    paymentMethod = "TNG";
                }
                else {
                    cout << RED << "Invalid payment method!" << RESET << endl;
                    continue; // Restart the loop to let the user re-enter
                }

                // Confirm purchase before proceeding
                cout << CYAN << "Do you confirm the purchase with the selected payment details? (Y/N): " << RESET;
                cin >> choice;
                if (choice == 'Y' || choice == 'y') {
                    // Proceed with the purchase and database update item table
                    db.prepareStatement("UPDATE item SET Item_Status = 'Sold' WHERE Item_ID = ?");
                    db.stmt->setInt(1, itemID);
                    db.stmt->executeUpdate();

                    // Transaction table update
                    Transaction(buyerID, sellerID, itemID, itemPrice, paymentMethod,buyerAddress);

                    system("cls");
                    cout << GREEN << "Purchase successful!" << RESET << endl;
                    cout << BYELLOW << "You bought: " << RESET << BCYAN << itemName << RESET << " for " << BCYAN << "RM " << fixed << setprecision(2) << totalPrice << " (included fee)" << RESET << endl;
                    break; // Exit the loop after successful purchase
                }
                else if (choice == 'N' || choice == 'n') {
                    system("cls");
                    cout << YELLOW << "Cancelled. Returning to user menu..." << RESET << endl;
                    return;
                }
                else {
                    cout << RED << "Invalid choice! Please enter 'Y' to confirm or 'N' to cancel." << RESET << endl;
                }
            }
            else if (choice == 'N' || choice == 'n') {
                system("cls");
                cout << YELLOW << "Cancelled. Returning to user menu..." << RESET << endl;
                return;
            }
            else {
                system("cls");
                cout << RED << "Invalid choice! Please enter 'Y' to confirm or 'N' to cancel." << RESET << endl;
            }
        }
    }
}

void Item::Transaction(int buyerid, int sellerid, int itemid, double itemPrice, string PaymentMethod, string buyerAddress){
    DBConnection db;

    // Step 1: Get the highest item_ID from the database
    db.prepareStatement("SELECT MAX(Transaction_ID) FROM transaction");
    db.QueryResult();
    int newTransactionID = 40001;  // Start Transaction_ID from 40001

    if (db.res->next()) {
        int maxTransactionID = db.res->getInt(1);
        if (maxTransactionID > 0) {
            newTransactionID = maxTransactionID + 1;  // Increment the highest User_ID by 1
        }
    }
    //Calculate Fee
    float fee = itemPrice * 0.03;

    // Auto fill in status
    time_t now = time(NULL);
    struct tm date = *localtime(&now);
    char currentDate[80];
    strftime(currentDate, sizeof(currentDate), "%Y-%m-%d", &date); // Get current date in YYYY-MM-DD format
    try {    
    // Step 3: Record the transaction in the transaction table
     db.prepareStatement("INSERT INTO transaction (Transaction_ID, Buyer_ID, Seller_ID, Item_ID, Amount,Fee, Payment_Method, Buyer_Address,Transaction_Date) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)");
      db.stmt->setInt(1, newTransactionID);
      db.stmt->setInt(2, buyerid);
      db.stmt->setInt(3, sellerid);
      db.stmt->setInt(4, itemid);
      db.stmt->setDouble(5, itemPrice);
      db.stmt->setDouble(6, fee);
      db.stmt->setString(7, PaymentMethod);
      db.stmt->setString(8, buyerAddress);
      db.stmt->setString(9, currentDate);
      db.stmt->executeUpdate();
      }

      catch (sql::SQLException& e) {
          cout << "# ERR: " << e.what();
          _getch();
      }
}

void Item::SearchAndFilter(const string& keyword, double minPrice, double maxPrice, int categoryID) {
    string query = "SELECT Item_ID, User_ID, Item_Name, Item_Description, Item_Price, Category_Name, Item_Post_Date, Item_Status FROM item JOIN category USING(Category_ID) WHERE Item_Status = 'Available'"; // Base query

    // Add keyword condition
    if (!keyword.empty()) {
        query += " AND (Item_Name LIKE '%" + keyword + "%' OR Item_Description LIKE '%" + keyword + "%')";
    }

    // Add price range condition
    if (minPrice >= 0 && maxPrice >= 0) {
        query += " AND Item_Price BETWEEN " + to_string(minPrice) + " AND " + to_string(maxPrice);
    }
    else if (minPrice >= 0 && maxPrice < 0) {
        // Only minimum price is provided
        query += " AND Item_Price >= " + to_string(minPrice);
    }
    else if (minPrice < 0 && maxPrice >= 0) {
        // Only maximum price is provided
        query += " AND Item_Price <= " + to_string(maxPrice);
    }

    // Add category condition
    if (categoryID >= 0) {
        query += " AND Category_ID = " + to_string(categoryID);
    }

    // Add sorting by Item_ID in ascending order
    query += " ORDER BY Item_ID ASC";

    // Execute the query
    DBConnection db;

    // Prepare and execute the SQL query
    db.prepareStatement(query);
    // Fetch the query result
    db.QueryResult();

    // Display the results
    cout << BYELLOW << left << setw(5) << "ID" << " | "
        << setw(22) << "Item Name" << " | "
        << setw(40) << "Description" << " | "
        << setw(15) << "Price (RM)" << " | "
        << setw(15) << "Category" << " | "
        << setw(11) << "Post Date" << " | " << RESET << endl;

    cout << YELLOW << string(125, '-') << RESET << endl;

    // Iterate over the results
    while (db.res->next()) {
        // Truncate Item Name and Item Description if they are too long
        string itemName = db.res->getString("Item_Name");
        string itemDescription = db.res->getString("Item_Description");

        if (itemName.length() > 18) {
            itemName = itemName.substr(0, 18) + "...";
        }

        if (itemDescription.length() > 35) {
            itemDescription = itemDescription.substr(0, 35) + "...";
        }

        // Display the item details
        cout << left << setw(5) << db.res->getInt("Item_ID") << " | "
            << BCYAN << setw(22) << itemName << RESET << " | "
            << setw(40) << itemDescription << " | "
            << setw(15) << fixed << setprecision(2) << db.res->getDouble("Item_Price") << " | "
            << setw(15) << db.res->getString("Category_Name") << " | "
            << setw(11) << db.res->getString("Item_Post_Date") << " | " << endl;
    }
    cout << YELLOW << string(125, '-') << RESET << endl;
    cout << endl;
}

void Item::SearchAndFilterSoldItem(const string& keyword, double minPrice, double maxPrice, int categoryID) {
    string query = "SELECT Item_ID, User_ID, Item_Name, Item_Description, Item_Price, Category_Name, Item_Post_Date, Item_Status FROM item JOIN category USING(Category_ID) WHERE Item_Status = 'Sold'"; // Base query

    // Add keyword condition
    if (!keyword.empty()) {
        query += " AND (Item_Name LIKE '%" + keyword + "%' OR Item_Description LIKE '%" + keyword + "%')";
    }

    // Add price range condition
    if (minPrice >= 0 && maxPrice >= 0) {
        query += " AND Item_Price BETWEEN " + to_string(minPrice) + " AND " + to_string(maxPrice);
    }
    else if (minPrice >= 0 && maxPrice < 0) {
        // Only minimum price is provided
        query += " AND Item_Price >= " + to_string(minPrice);
    }
    else if (minPrice < 0 && maxPrice >= 0) {
        // Only maximum price is provided
        query += " AND Item_Price <= " + to_string(maxPrice);
    }

    // Add category condition
    if (categoryID >= 0) {
        query += " AND Category_ID = " + to_string(categoryID);
    }

    // Add sorting by Item_ID in ascending order
    query += " ORDER BY Item_ID ASC";

    // Execute the query
    DBConnection db;

    // Prepare and execute the SQL query
    db.prepareStatement(query);
    // Fetch the query result
    db.QueryResult();

    // Display the results
    cout << BYELLOW << left << setw(5) << "ID" << " | "
        << setw(22) << "Item Name" << " | "
        << setw(40) << "Description" << " | "
        << setw(15) << "Price (RM)" << " | "
        << setw(15) << "Category" << " | "
        << setw(11) << "Post Date" << " | " << RESET << endl;

    cout << YELLOW << string(125, '-') << RESET << endl;

    // Iterate over the results
    while (db.res->next()) {
        // Truncate Item Name and Item Description if they are too long
        string itemName = db.res->getString("Item_Name");
        string itemDescription = db.res->getString("Item_Description");

        if (itemName.length() > 18) {
            itemName = itemName.substr(0, 18) + "...";
        }

        if (itemDescription.length() > 35) {
            itemDescription = itemDescription.substr(0, 35) + "...";
        }

        // Display the item details
        cout << left << setw(5) << db.res->getInt("Item_ID") << " | "
            << BCYAN << setw(22) << itemName << RESET << " | "
            << setw(40) << itemDescription << " | "
            << setw(15) << fixed << setprecision(2) << db.res->getDouble("Item_Price") << " | "
            << setw(15) << db.res->getString("Category_Name") << " | "
            << setw(11) << db.res->getString("Item_Post_Date") << " | " << endl;
    }
    cout << YELLOW << string(125, '-') << RESET << endl;
    cout << endl;
}

void Item::generateMonthlyProfitFeeReport() {
    system("cls"); // Clear the console
    cout << GREEN << "==============================================" << RESET << endl;
    cout << CYAN << "         PROFIT FEE REPORT (MONTHLY)         " << RESET << endl;
    cout << GREEN << "==============================================" << RESET << endl << endl;

    int year = 0, month = 0;

    // Get year input with validation
    while (true) {
        cout << BYELLOW << "Enter year (e.g., 2024): " << RESET;
        cin >> year;

        if (cin.fail()) { // Check if the input is not a number
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
            cerr << RED << "Invalid input! Please enter a numeric year (e.g., 2024)." << RESET << endl;
        }
        else if (year < 2000 || year > 2100) { // Validate year range
            cerr << RED << "Invalid year! Please enter a value between 2000 and 2100." << RESET << endl;
        }
        else {
            break; // Valid year input
        }
    }

    // Get month input with validation
    while (true) {
        cout << BYELLOW << "Enter month (1-12): " << RESET;
        cin >> month;

        if (cin.fail()) { // Check if the input is not a number
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
            cerr << RED << "Invalid input! Please enter a numeric month (1-12)." << RESET << endl;
        }
        else if (month < 1 || month > 12) { // Validate month range
            cerr << RED << "Invalid month! Please enter a value between 1 and 12." << RESET << endl;
        }
        else {
            break; // Valid month input
        }
    }

    DBConnection db;

    try {
        db.prepareStatement(
            "SELECT c.Category_Name, c.Category_ID, SUM(t.Fee) AS TotalFee FROM transaction t "
            "JOIN item i ON t.Item_ID = i.Item_ID "
            "JOIN category c ON i.Category_ID = c.Category_ID "
            "WHERE MONTH(t.Transaction_Date) = ? AND YEAR(t.Transaction_Date) = ? "
            "AND t.Transaction_Status = 'Completed' " // Added condition to check for completed transactions
            "GROUP BY c.Category_Name "
            "ORDER BY c.Category_ID ASC");

        // Bind user inputs to the query
        db.stmt->setInt(1, month);
        db.stmt->setInt(2, year);
        db.QueryResult();

        if (!db.res->next()) { // Check if query returned no results
            cout << RED << "No report exists for " << setw(2) << month << "/" << year << "." << RESET << endl;
            return;
        }

        // Reset result cursor for data processing
        db.QueryResult();
        double totalProfit = 0.0;

        // Calculate total profit
        while (db.res->next()) {
            double totalFee = db.res->getDouble("TotalFee");
            totalProfit += totalFee;
        }

        // Reset result cursor to display data
        db.QueryResult();
        system("cls"); // Clear the console
        cout << GREEN << "==============================================" << RESET << endl;
        cout << CYAN << "         PROFIT FEE REPORT (MONTHLY)         " << RESET << endl;
        cout << GREEN << "==============================================" << RESET << endl << endl;
        cout << CYAN << "Profit Fee Summary for " << RESET << BYELLOW << month << "/" << year << CYAN << ":" << RESET << endl;
        cout << string(70, '-') << endl;
        cout << left << setw(15) << "Category" << " | "
            << setw(15) << "Total Fee (RM)" << " | "
            << setw(23) << "Percentage of Total Profit (%)" << endl;
        cout << string(70, '-') << endl;

        // Print report
        while (db.res->next()) {
            string categoryName = db.res->getString("Category_Name");
            double totalFee = db.res->getDouble("TotalFee");
            double percentage = (totalProfit != 0) ? (totalFee / totalProfit) * 100 : 0.0;
            string bar = string(static_cast<int>(percentage * 0.25), '#');

            cout << left << setw(15) << categoryName
                << " | " << setw(15) << fixed << setprecision(2) << totalFee
                << " | " << setw(5) << fixed << setprecision(2) << percentage << " " << bar  << endl;
        }

        cout << string(70, '-') << endl << endl;
        // Display total profit fee
        cout << BCYAN << "Total Profit Fee in " << RESET << BYELLOW << month << "/" << year << BCYAN << ": "<< RESET << BYELLOW << "RM" << fixed << setprecision(2) << totalProfit << RESET << endl;
        cout << endl;

    }
    catch (sql::SQLException& e) {
        cerr << RED << "Error generating profit fee report: " << e.what() << RESET << endl;
    }
}

void Item::generateAnnualProfitFeeReport() {
    system("cls");
    cout << GREEN << "==============================================" << RESET << endl;
    cout << CYAN << "           PROFIT FEE REPORT (ANNUAL)        " << RESET << endl;
    cout << GREEN << "==============================================" << RESET << endl << endl;

    int year = 0;

    // Get year input with validation
    while (true) {
        cout << BYELLOW << "Enter year (e.g., 2024): " << RESET;
        cin >> year;

        if (cin.fail()) { // Check if the input is not a number
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
            cerr << RED << "Invalid input! Please enter a numeric year (e.g., 2024)." << RESET << endl;
        }
        else if (year < 2000 || year > 2100) { // Validate year range
            cerr << RED << "Invalid year! Please enter a value between 2000 and 2100." << RESET << endl;
        }
        else {
            break; // Valid year input
        }
    }

    DBConnection db;

    try {
        db.prepareStatement(
            "SELECT MONTH(t.Transaction_Date) AS Month, SUM(t.Fee) AS TotalFee "
            "FROM transaction t "
            "WHERE YEAR(t.Transaction_Date) = ? "
            "AND t.Transaction_Status = 'Completed' " // Added condition to check for completed transactions
            "GROUP BY MONTH(t.Transaction_Date) "
            "ORDER BY Month ASC");

        // Bind user input to the query
        db.stmt->setInt(1, year);
        db.QueryResult();

        double monthlyFees[12] = { 0 }; // Array to store total fees for each month
        double totalProfit = 0.0;
        double maxMonthlyFee = 0.0; // To store the maximum monthly fee for scaling the bar chart

        while (db.res->next()) {
            int month = db.res->getInt("Month");
            double fee = db.res->getDouble("TotalFee");
            monthlyFees[month - 1] = fee;
            totalProfit += fee;
            maxMonthlyFee = max(maxMonthlyFee, fee); // Update the max monthly fee
        }

        if (totalProfit == 0.0) { // Check if no transactions exist
            cout << RED << "No transactions found for the year " << year << "." << RESET << endl;
            return;
        }

        system("cls");
        cout << GREEN << "==============================================" << RESET << endl;
        cout << CYAN << "           PROFIT FEE REPORT (ANNUAL)        " << RESET << endl;
        cout << GREEN << "==============================================" << RESET << endl << endl;
        cout << CYAN << "Profit Fee Summary for " << RESET << BYELLOW << year << CYAN << ":" << RESET << endl;
        cout << string(70, '-') << endl;
        cout << left << setw(6) << "Month" << " | "
            << setw(15) << "Total Fee (RM)" << " | "
            << setw(30) << "Percentage of Annual Profit (%)" << endl;
        cout << string(70, '-') << endl;

        for (int i = 0; i < 12; i++) {
            double percentage = (totalProfit != 0) ? (monthlyFees[i] / totalProfit) * 100 : 0.0;

            // Scale the bar length based on the maximum monthly fee instead of total annual profit
            double scaledPercentage = (maxMonthlyFee != 0) ? (monthlyFees[i] / maxMonthlyFee) * 100 : 0.0;
            string bar = string(static_cast<int>(scaledPercentage * 0.25), '#'); // Adjust multiplier as needed
            cout << left << setw(6) << (i + 1)
                << " | " << setw(15) << fixed << setprecision(2) << monthlyFees[i]
                << " | " << setw(5) << fixed << setprecision(2) << percentage << " " << bar << endl;
        }

        cout << string(70, '-') << endl << endl;
        cout << BCYAN << "Total Annual Profit Fee: " << fixed << setprecision(2) << RESET << BYELLOW << "RM" << totalProfit << RESET << endl;

    }
    catch (sql::SQLException& e) {
        cerr << RED << "Error generating annual profit fee report: " << e.what() << RESET << endl;
    }
}

void Item::generateAnnualTransactionReport() {
    system("cls"); // Clear the console
    cout << GREEN << "====================================================" << RESET << endl;
    cout << CYAN << "             TRANSACTION REPORT (ANNUAL)   " << RESET << endl;
    cout << GREEN << "====================================================" << RESET << endl << endl;

    int year = 0;

    // Get year input with validation
    while (true) {
        cout << BYELLOW << "Enter year (e.g., 2024): " << RESET;
        cin >> year;

        if (cin.fail()) { // Check if the input is not a number
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
            cerr << RED << "Invalid input! Please enter a numeric year (e.g., 2024)." << RESET << endl;
        }
        else if (year < 2000 || year > 2100) { // Validate year range
            cerr << RED << "Invalid year! Please enter a value between 2000 and 2100." << RESET << endl;
        }
        else {
            break; // Valid year input
        }
    }

    DBConnection db;

    try {
        db.prepareStatement(
            "SELECT c.Category_Name, c.Category_ID, COUNT(t.Transaction_ID) AS TotalTransactions "
            "FROM transaction t "
            "JOIN item i ON t.Item_ID = i.Item_ID "
            "JOIN category c ON i.Category_ID = c.Category_ID "
            "WHERE YEAR(t.Transaction_Date) = ? "
            "AND t.Transaction_Status = 'Completed' " // Added condition to check for completed transactions
            "GROUP BY c.Category_Name "
            "ORDER BY c.Category_ID ASC");

        // Bind the year to the query
        db.stmt->setInt(1, year);
        db.QueryResult();

        int totalTransactions = 0;
        vector<pair<string, int>> categoryData; // Store category name and transaction count
        int maxTransactions = 0; // To scale the bar chart

        while (db.res->next()) {
            string categoryName = db.res->getString("Category_Name");
            int numTransactions = db.res->getInt("TotalTransactions");
            totalTransactions += numTransactions;
            categoryData.push_back({ categoryName, numTransactions });
            maxTransactions = max(maxTransactions, numTransactions);
        }

        if (totalTransactions == 0) { // Check if no transactions exist
            cout << RED << "No transactions found for the year " << year << "." << RESET << endl;
            return;
        }

        system("cls");
        cout << GREEN << "====================================================" << RESET << endl;
        cout << CYAN << "             TRANSACTION REPORT (ANNUAL)   " << RESET << endl;
        cout << GREEN << "====================================================" << RESET << endl << endl;
        cout << CYAN << "Transaction Summary for " << RESET << BYELLOW << year << CYAN << ":" << RESET << endl;
        cout << string(80, '-') << endl;
        cout << left << setw(20) << "Category" << " | "
            << setw(15) << "Transactions" << " | "
            << setw(30) << "Percentage (%)" << endl;
        cout << string(80, '-') << endl;

        for (const auto& category : categoryData) {
            string categoryName = category.first;
            int numTransactions = category.second;
            double percentage = (totalTransactions != 0) ? (numTransactions / static_cast<double>(totalTransactions)) * 100 : 0.0;

            // Scale the bar length based on the maximum transaction count
            double scaledPercentage = (maxTransactions != 0) ? (numTransactions / static_cast<double>(maxTransactions)) * 100 : 0.0;
            string bar = string(static_cast<int>(scaledPercentage * 0.25), '#'); // Adjust multiplier for bar length
            cout << left << setw(20) << categoryName
                << " | " << setw(15) << numTransactions
                << " | " << setw(5) << fixed << setprecision(2) << percentage << " " << bar << endl;
        }

        cout << string(80, '-') << endl << endl;
        cout << BCYAN << "Total Transactions: " << RESET << BYELLOW << totalTransactions << RESET << endl;

    }
    catch (sql::SQLException& e) {
        cerr << RED << "Error generating annual transaction report: " << e.what() << RESET << endl;
    }
}

void Item::MyProfile(int userID) {
    DBConnection db;  // Database connection object

    try {
        // Query user profile details from the user table
        db.prepareStatement(
            "SELECT * "
            "FROM user "
            "WHERE User_ID = ?");

        // Bind the userID to the query
        db.stmt->setInt(1, userID);
        db.QueryResult();

        if (db.res->next()) {
            // Fetch the user details
            int userId = db.res->getInt("User_ID");
            string username = db.res->getString("Username");
            string email = db.res->getString("Email");
            string phone = db.res->getString("Phone");
            string name = db.res->getString("Name");
			string address = db.res->getString("Address");

            // Display user profile details
            cout << GREEN << "==============================================" << RESET << endl;
            cout << CYAN << "                USER PROFILE                 " << RESET << endl;
            cout << GREEN << "==============================================" << RESET << endl << endl;

            cout << BCYAN << string(50, '-') << RESET << endl;
            cout << BYELLOW << "Personal Information:" << endl;
            cout << CYAN << "User ID: " << BWHITE << userId << endl;
            cout << CYAN << "Username: " << BWHITE << username << endl;
            cout << CYAN << "Email: " << BWHITE << email << endl;
            cout << CYAN << "Phone: " << BWHITE << phone << endl;
            cout << CYAN << "Name: " << BWHITE << name << endl;
			cout << CYAN << "Address: " << BWHITE << address << RESET << endl;
            cout << BCYAN << string(50, '-') << RESET << endl;

            // Query number of items uploaded by the user
            db.prepareStatement(
                "SELECT COUNT(*) AS ItemCount FROM item "
                "WHERE User_ID = ?");

            db.stmt->setInt(1, userID);
            db.QueryResult();

            int itemCount = 0;
            if (db.res->next()) {
                itemCount = db.res->getInt("ItemCount");
            }

            // Display number of uploaded items
            cout << GREEN <<  "Number of Items Uploaded: " << itemCount << endl;

            // Query the number of sold items and total earnings (from transactions)
            db.prepareStatement(
                "SELECT COUNT(*) AS SoldItems, SUM(t.Amount) AS TotalEarnings "
                "FROM transaction t "
                "WHERE t.Seller_ID = ? AND t.Transaction_Status = 'Completed'");

            db.stmt->setInt(1, userID);
            db.QueryResult();

            int soldItems = 0;
            double totalEarnings = 0.0;
            if (db.res->next()) {
                soldItems = db.res->getInt("SoldItems");
                totalEarnings = db.res->getDouble("TotalEarnings");
            }

            // Display sold items and total earnings
            cout << RED << "Number of Items Sold: " << soldItems << endl;
            cout << GREEN << "Total Earnings: RM " << fixed << setprecision(2) << totalEarnings << endl;
            cout << BCYAN << string(50, '-') << RESET << endl;

        }
        else {
            cout << RED << "No user found with User ID: " << userID << RESET << endl;
        }

    }
    catch (sql::SQLException& e) {
        cerr << RED << "Error fetching user profile data: " << e.what() << RESET << endl;
    }
}

void Item::UserEditPassword(int userID, string oldPassword) {
    system("cls");  // Clear the console screen
    DBConnection db;  // Database connection object
    string newPassword, confirmPassword;

    try {
        // Query the current password of the user
        db.prepareStatement(
            "SELECT Password FROM user WHERE User_ID = ?");

        db.stmt->setInt(1, userID);
        db.QueryResult();

        string currentPassword;
        if (db.res->next()) {
            currentPassword = db.res->getString("Password");
        }
        else {
            cout << RED << "User not found!" << RESET << endl;
            return;
        }

        // Step 1: Verify the old password
        if (oldPassword != currentPassword) {
            cout << RED << "Incorrect old password! Password change failed." << RESET << endl;
            return;
        }

        // Step 2: Prompt the user to input the new password
        while (true) {
            cout << BYELLOW << "Enter your new password: " << RESET;            
            cin.clear();  // Clear any error flags
            getline(cin, newPassword);

            // Step 3: Prompt the user to confirm the new password
            cout << BYELLOW << "Confirm your new password: " << RESET;
			getline(cin, confirmPassword);

            if (newPassword != confirmPassword) {
                cout << RED << "Passwords do not match! Please try again." << RESET << endl;
            }
            else if (newPassword.empty()) {
                cout << RED << "Error: Password cannot be empty!" << RESET << endl;
                return;
            }
            else if (newPassword.find(' ') != string::npos) {
                cout << RED << "Error: Password cannot contain spaces!" << RESET << endl;
                return;
            }
            else {
                break;  // If passwords match, break the loop
            }
        }

        // Step 4: Update the password in the database
        db.prepareStatement(
            "UPDATE user SET Password = ? WHERE User_ID = ?");

        db.stmt->setString(1, newPassword);
        db.stmt->setInt(2, userID);
        db.QueryResult();  // Execute the update query

        // Confirmation message
        system("cls");
        cout << GREEN << "Password updated successfully!" << RESET << endl;

    }
    catch (sql::SQLException& e) {
        cerr << RED << "Error updating password: " << e.what() << RESET << endl;
    }
}

void Item::UserEditEmail(int userID, string newEmail) {
    system("cls");  // Clear the console screen
    DBConnection db;  // Database connection object
  
    if (newEmail.empty()) {
        cout << RED << "Error: Email cannot be empty!" << RESET << endl;
        return;
    }
    if (newEmail.find('@') == string::npos) {
        cout << RED << "Error: Email must contain '@'!" << RESET << endl;
        return;
    }
    if (newEmail.find(".com") == string::npos) {
        cout << RED << "Error: Email must contain '.com'!" << RESET << endl;
        return;
    }
    if (newEmail.find(' ') != string::npos) {
        cout << RED << "Error: Email cannot contain spaces!" << RESET << endl;
        return;
    }

    try {
        // Step 1: Check if the new email is already in use by another user (excluding current user)
        db.prepareStatement(
            "SELECT User_ID FROM user WHERE Email = ? AND User_ID != ?");

        db.stmt->setString(1, newEmail);  // Bind the new email
        db.stmt->setInt(2, userID);  // Bind the current user ID to exclude them from the check
        db.QueryResult();


        if (db.res->next()) {
            cout << RED << "This email is already in use by another user!" << RESET << endl;
            return;  // If the email is already taken, stop the process
        }

        // Step 2: Update the email in the database
        db.prepareStatement(
            "UPDATE user SET Email = ? WHERE User_ID = ?");

        db.stmt->setString(1, newEmail);  // Bind the new email
        db.stmt->setInt(2, userID);  // Bind the user ID
        db.QueryResult();  // Execute the update query

        // Step 3: Confirm that the email was successfully updated
        cout << GREEN << "Email updated successfully!" << RESET << endl;

    }
    catch (sql::SQLException& e) {
        cerr << RED << "Error updating email: " << e.what() << RESET << endl;
    }
}

void Item::UserEditPhone(int userID, string newPhone) {
    system("cls");
    DBConnection db;  // Database connection object

    if (newPhone.empty()) {
        cout << RED << "Error: Phone Number cannot be empty!" << RESET << endl;
        return;
    }
    if (newPhone.find(' ') != string::npos) {
        cout << RED << "Error: Phone Number cannot contain spaces!" << RESET << endl;
        return;
    }
    if (newPhone.length() < 10 || newPhone.length() > 11) {
        cout << RED << "Error: Phone Number is invalid!" << RESET << endl;
        return;
    }

    try {
        // Step 1: Check if the new phone number is already in use by another user (excluding current user)
        db.prepareStatement(
            "SELECT User_ID FROM user WHERE Phone = ? AND User_ID != ?");

        db.stmt->setString(1, newPhone);  // Bind the new phone number
        db.stmt->setInt(2, userID);  // Bind the current user ID to exclude them from the check
        db.QueryResult();

        if (db.res->next()) {
            cout << RED << "This phone number is already in use by another user!" << RESET << endl;
            return;  // If the phone number is already taken, stop the process
        }

        // Step 2: Update the phone number in the database
        db.prepareStatement(
            "UPDATE user SET Phone = ? WHERE User_ID = ?");

        db.stmt->setString(1, newPhone);  // Bind the new phone number
        db.stmt->setInt(2, userID);  // Bind the user ID
        db.QueryResult();  // Execute the update query

        // Step 3: Confirm that the phone number was successfully updated
        cout << GREEN << "Phone number updated successfully!" << RESET << endl;

    }
    catch (sql::SQLException& e) {
        cerr << RED << "Error updating phone number: " << e.what() << RESET << endl;
    }
}

void Item::UserEditAddress(int userID, string newAddress) {
	system("cls");
	DBConnection db;  // Database connection object

	if (newAddress.empty()) {
		cout << RED << "Error: Address cannot be empty!" << RESET << endl;
		return;
	}

	try {
		// Step 1: Update the address in the database
		db.prepareStatement(
			"UPDATE user SET Address = ? WHERE User_ID = ?");

		db.stmt->setString(1, newAddress);  // Bind the new address
		db.stmt->setInt(2, userID);  // Bind the user ID
		db.QueryResult();  // Execute the update query

		// Step 2: Confirm that the address was successfully updated
		cout << GREEN << "Address updated successfully!" << RESET << endl;

	}
	catch (sql::SQLException& e) {
		cerr << RED << "Error updating address: " << e.what() << RESET << endl;
	}
}

void Item::ConfirmTransactionCompletion(int transactionID, int userid) {
    DBConnection db;

    try {
        // Check if the transaction exists and is still pending
        db.prepareStatement("SELECT * FROM transaction WHERE Transaction_ID = ? AND Buyer_ID = ?");
        db.stmt->setInt(1, transactionID);
        db.stmt->setInt(2, userid);
        db.QueryResult();

        if (db.res->next()) {
            string currentStatus = db.res->getString("Transaction_Status");
            int itemID = db.res->getInt("Item_ID"); // Get the associated item ID

            if (currentStatus == "Pending") {
                // Ask the user whether they want to complete or cancel the transaction
                int choice;
                cout << BYELLOW << "\nTransaction ID " << transactionID << " is currently " << RESET << BCYAN << "'Pending'"<< BYELLOW << ".\n" << RESET;
                cout << BWHITE << "1. Mark as Completed\n";
                cout << BWHITE << "2. Cancel the Transaction\n";
                cout << BWHITE << "0. Back to the menu\n\n";
                STATUSCHOICE:
                cout << BYELLOW << "Enter your choice: " << RESET;
                cin >> choice;

                if (cin.fail() || cin.peek() != '\n') {
                    cin.clear(); // Clear error flag
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                    cout << RED << "Invalid choice! Please try again!\n" << RESET;
                    goto STATUSCHOICE;
                }

                if (choice == 0) {
                    // Go back to the Pending Transactions menu
                    return;
                }

                string newStatus;
                if (choice == 1) {
                    newStatus = "Completed";
                }
                else if (choice == 2) {
                    newStatus = "Canceled";

                    // Update the associated item's status back to "Available"
                    db.prepareStatement("UPDATE item SET Item_Status = 'Available' WHERE Item_ID = ?");
                    db.stmt->setInt(1, itemID);
                    db.stmt->executeUpdate();

                    // Delete the transaction from the database
                    db.prepareStatement("DELETE FROM transaction WHERE Transaction_ID = ?");
                    db.stmt->setInt(1, transactionID);
                    db.stmt->executeUpdate();

                }
                else {
                    cout << RED << "Invalid choice. Transaction status remains unchanged.\n" << RESET;
                    return;
                }

                // Update the transaction status in the database
                db.prepareStatement("UPDATE transaction SET Transaction_Status = ? WHERE Transaction_ID = ?");
                db.stmt->setString(1, newStatus);
                db.stmt->setInt(2, transactionID);
                db.stmt->executeUpdate();

                cout << BCYAN << "\nTransaction ID " << transactionID << " status updated to '" << newStatus << "'.\n" << RESET;
            }
            else if (currentStatus == "Completed") {
                cout << BCYAN << "Transaction ID " << transactionID << " is already marked as 'Completed'.\n" << RESET;
            }
            else if (currentStatus == "Canceled") {
                cout << RED << "Transaction ID " << transactionID << " has already been 'Canceled'.\n" << RESET;
            }
            else {
                cout << RED << "Transaction ID " << transactionID << " has an unknown status: " << currentStatus << ".\n" << RESET;
            }
        }
        else {

        }
    }
    catch (sql::SQLException& e) {
        cout << "# ERR: " << e.what() << endl;
    }
}

void Item::ViewPendingTransactions(int userid) {
    DBConnection db;

    try {
        // Query to fetch all pending transactions and associated item details
        db.prepareStatement(
            "SELECT t.Transaction_ID, t.Item_ID, i.Item_Name, i.Item_Description, t.Transaction_Status "
            "FROM transaction t "
            "JOIN item i ON t.Item_ID = i.Item_ID "
            "WHERE t.Buyer_ID = ? AND t.Transaction_Status = 'Pending'"
        );
        db.stmt->setInt(1, userid);
        db.QueryResult();

        cout << BCYAN << "\n--- Pending Transactions ---\n\n";
        cout << YELLOW << string(128, '-') << RESET << endl;
        // Display table header
        cout << BYELLOW << left << setw(15) << "Transaction ID" << " | "
            << setw(10) << "Item ID" << " | "
            << setw(30) << "Item Name" << " | "
            << setw(51) << "Description" << " | "
            << setw(8) << "Status" << " | " << RESET << endl;

        cout << YELLOW << string(128, '-') << RESET << endl;

        bool hasPendingTransactions = false;

        while (db.res->next()) {
            hasPendingTransactions = true;  // Mark that there is at least one pending transaction

            int transactionID = db.res->getInt("Transaction_ID");
            int itemID = db.res->getInt("Item_ID");
            string itemName = db.res->getString("Item_Name");
            string itemDescription = db.res->getString("Item_Description");
            string transactionStatus = db.res->getString("Transaction_Status");

            // Truncate item name and description if too long
            if (itemName.length() > 25) {
                itemName = itemName.substr(0, 25) + "...";
            }
            if (itemDescription.length() > 48) {
                itemDescription = itemDescription.substr(0, 48) + "...";
            }

            // Display transaction details
            cout << left << setw(15) << transactionID << " | "
                << setw(10) << itemID << " | "
                << setw(30) << itemName << " | "
                << setw(51) << itemDescription << " | "
                << setw(8) << transactionStatus << " | " << endl;
        }

        cout << YELLOW << string(128, '-') << RESET << endl;

        if (!hasPendingTransactions) {
            cout << "No pending transactions found." << endl;
        }
    }
    catch (sql::SQLException& e) {
        cout << "# ERR: " << e.what() << endl;
    }
}

void Item::displayTransactionItemDetails(int transaction_ID, int userID) {
    DBConnection db;

    try {
        // Query to fetch transaction details along with item details
        db.prepareStatement(
            "SELECT i.Item_ID, i.Item_Name, i.Item_Description, i.Item_Price, c.Category_Name, "
            "i.Item_Post_Date, i.Item_Status, t.Transaction_Status, t.Buyer_Address "
            "FROM item i "
            "JOIN category c ON i.Category_ID = c.Category_ID "
            "JOIN transaction t ON i.Item_ID = t.Item_ID "
            "WHERE t.Transaction_ID = ? AND t.Buyer_ID = ?"
        );
        db.stmt->setInt(1, transaction_ID);
        db.stmt->setInt(2, userID);
        db.QueryResult();

        if (db.res->next()) {
            // Extract data from the query result
            string itemName = db.res->getString("Item_Name");
            double itemPrice = db.res->getDouble("Item_Price");
            double fee = itemPrice * 0.03;
            double totalPrice = fee + itemPrice;
            string buyerAddress = db.res->getString("Buyer_Address");  // Get Buyer Address

            // Clear the screen and display item details
            system("cls");
            cout << GREEN << "========================================" << RESET << endl;
            cout << CYAN << "              ITEM DETAILS               " << RESET << endl;
            cout << GREEN << "========================================" << RESET << endl << endl;
            cout << YELLOW << string(150, '*') << RESET << endl;
            cout << BWHITE << "Transaction ID: " <<BCYAN<< transaction_ID << RESET << endl;
            cout << "Item ID: " << BCYAN << db.res->getInt("Item_ID") << RESET << endl;
            cout << "Item Name: " << itemName << endl;
            cout << "Item Description: " << db.res->getString("Item_Description") << endl;
            cout << "Category: " << db.res->getString("Category_Name") << endl;
            cout << BWHITE << "Item Post Date: " << db.res->getString("Item_Post_Date") << RESET << endl;            
            cout << BWHITE << "Delivery Address: " << RESET << BCYAN << buyerAddress << RESET << endl;
            cout << BCYAN << string(60, '-') << RESET << endl;
            cout << RED << "Item Price: RM" << fixed << setprecision(2) << itemPrice << RESET << endl;
            cout << RED << "3% Platform Fee: RM" << fixed << setprecision(2) << fee << endl << RESET;
            cout << GREEN << "Total Price: RM" << fixed << setprecision(2) << totalPrice << RESET << endl;
            cout << BCYAN << string(60, '-') << RESET << endl;
            cout << RESET << RED << "Transaction Status: " << BCYAN << db.res->getString("Transaction_Status") << RESET << endl;
            cout << BCYAN << string(60, '-') << RESET << endl;
            cout << YELLOW << string(150, '*') << RESET << endl << endl;
        }
        else {
            // Handle case where no matching transaction is found
            system("cls");
            cout << RED << "Transaction not found or you are not authorized to view it!" << RESET << endl;
        }
    }
    catch (sql::SQLException& e) {
        cout << "# ERR: " << e.what() << endl;
    }
}


