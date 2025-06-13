#ifndef MENU_H
#define MENU_H

#include <vector>  // Include the vector header
#include <string>  // Include string header if you're using string

using namespace std;

class Menu {
public:
	bool login(string username, string password);
	void registerUser();
	void forgotPassword(string username, string email);
	void userMenu(int userid);
	void adminMenu();
	void displayMainMenu();
	
};
#endif // MENU_H