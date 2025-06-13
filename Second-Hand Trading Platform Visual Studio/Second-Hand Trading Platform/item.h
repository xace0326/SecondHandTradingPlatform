#ifndef ITEM_H
#define ITEM_H
using namespace std;

#include <vector>  // Include the vector header
#include <string>  // Include string header if you're using string

class Item {

public:
	void addItem(int userid, string itemName, string itemDescription, double itemPrice, int category);
	void removeItem(int itemID, int userID);
	void adminRemoveItem(int itemID);
	void displayUserAvailableItemList(int userID);
	void displayUserSoldItemList(int userID);	
	void displayUserPurchaseItemList(int userID);
	void displayAllItemList();
	void displaySoldItemList();
	void displayItemDetails(int itemID, int userID);
	void displayTransactionItemDetails(int transaction_ID, int userID);
	void AdminDisplayItemDetails(int itemID);
	void AdminDisplaySoldItemDetails(int itemID);
	void displaySoldItemDetails(int itemID, int userID);
	void displayPurchaseItemDetails(int itemID, int userID);
	void buyItem(int buyerID, int itemID);
	void Transaction(int buyerid, int sellerid, int itemid, double itemPrice, string PaymentMethod, string buyerAddress);
	void SearchAndFilter(const string& keyword, double minPrice, double maxPrice, int categoryID);
	void SearchAndFilterSoldItem(const string& keyword, double minPrice, double maxPrice, int categoryID);
	void generateMonthlyProfitFeeReport();
	void generateAnnualProfitFeeReport();
	void generateAnnualTransactionReport();
	void MyProfile(int userid);
	void UserEditPassword(int userid, string oldPassword);
	void UserEditEmail(int userid, string newEmail);
	void UserEditPhone(int userid, string newPhone);
	void UserEditAddress(int userid, string newAddress);
	void ConfirmTransactionCompletion(int transactionID, int userid);
	void ViewPendingTransactions(int userid);
};
#endif 