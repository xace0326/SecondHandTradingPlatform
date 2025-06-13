#include <iostream>
#include "db.h"
#include <mysql/jdbc.h>
#include <conio.h>

using namespace std;
using namespace sql;

DBConnection::DBConnection() {

	try {
		mysql::MySQL_Driver* driver = mysql::get_mysql_driver_instance();

		connection = driver->connect("localhost", "root", "q1w2e3r4");
		//database address, username, password

		connection->setSchema("second-hand_trading_platform"); //database name
		stmt = nullptr;
		res = nullptr;

	}
	catch (sql::SQLException& e) {
		if (e.getErrorCode() == 0) {
			system("cls");
			cout << "Unable to connect to database";
			_getch();
			exit(0);
			return;
		}
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		_getch();
	}
}

DBConnection::~DBConnection(){
	try {
		if (connection) {
			connection->close();
		}
		if (stmt) {
			stmt->close();
		}
		if (res) {
			res->close();
		}
	}
	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		_getch();
	}
}

void DBConnection::prepareStatement(string query) {
	stmt = connection->prepareStatement(query);
}

void DBConnection::QueryStatement() {
	try {
		stmt->executeUpdate();
	}
	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		_getch();
	}
}

void DBConnection::QueryResult() {

	try {
		res = stmt->executeQuery();
	}
	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		_getch();
	}
}