#pragma once

#include "User.h"
#include "Account.h"

class Admin : public User, public Account {
public:
	void addBook();
	void deleteBook();
	void viewAllReaders();
	void viewAllBorrowers();
	void adminMenu(bool& exitMenu, Admin& admin);
};