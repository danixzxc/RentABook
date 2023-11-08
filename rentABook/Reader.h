#pragma once


#include "User.h"
#include "Account.h"

class Reader : public User, public Account {
public:
	void borrowBook(string readerName);
	void returnBook(string readerName);
	void viewBorrowedBooks();
	void readerMenu(bool& exitMenu, Reader& reader);
};
