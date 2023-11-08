#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <locale>

#include "User.h";
#include "Book.h"


const string ADMIN_ACCOUNTS_FILE = "admin_account.txt";
const string READER_ACCOUNTS_FILE = "reader_account.txt";

using namespace std;

class Account {
private:
	void loginCheck(string fileName, string salt, string password, string login);
protected:
	string login;
	string password;
	size_t hashedPassword;
	string salt;
	bool isLogged = false;
public:
	void createAccount(string fileName);
	void logInAccount(string fileName);
	string validInput(string type);
	void logOut();
	bool getLogged();
	string getLogin();
};


