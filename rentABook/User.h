#pragma once
#include "Book.h"


class User {
protected:
	void vectorToFile(vector<Book> books);
private:
	void SortBooksByCriteria(vector<Book> &books, string criteria);
public:
	vector<Book> booksFileToVector();
	string safeStringInput();
	int safeIntInput();
	void printBooks(vector<Book>& books);
	void viewAllBooks();
	void sortBooks();
	void searchForABook();
	void guestMenu();
};