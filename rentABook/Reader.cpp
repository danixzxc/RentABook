#include "Account.h";
#include "Book.h";
#include "Reader.h";

#include <conio.h>
#include <regex>
#include <sstream>
#include <iomanip>
#include <random>


const string LIST_OF_BOOKS_FILE = "list_of_books.txt";
const string LIST_OF_BORROWERS_FILE = "list_of_borrowers.txt";
const string VALID_INPUT_INT = "int";
const char FIRST_SEPARATOR = ':';
const char SECOND_SEPARATOR = ',';
const int SALT_LENGTH = 8;

void addBookToBorrower(string readerName, string bookName)
{
	ifstream borrowersFile(LIST_OF_BORROWERS_FILE);
	string line;
	vector<string> borrowerLists;
	bool readerFound = false;
	while (getline(borrowersFile, line)) {
		stringstream ss(line);
		string borrower, borrowerList;
		getline(ss, borrower, FIRST_SEPARATOR);
		getline(ss, borrowerList);

		if (borrower == readerName) {
			readerFound = true;
			borrowerList += SECOND_SEPARATOR + bookName;
		}

		borrowerLists.push_back(borrower + FIRST_SEPARATOR + borrowerList);
	}
	borrowersFile.close();

	if (!readerFound) {
		borrowerLists.push_back(readerName + FIRST_SEPARATOR + " " + bookName);
	}

	ofstream borrowersFileOut(LIST_OF_BORROWERS_FILE);
	for (auto const& borrowerList : borrowerLists) {
		borrowersFileOut << borrowerList << endl;
	}
	borrowersFileOut.close();
}

void deleteBookFromBorrower(string readerName, string bookName)
{
	ifstream borrowersFile(LIST_OF_BORROWERS_FILE);
	ofstream tempBorrowersFile("temp.txt");

	bool readerFound = false;
	string line;

	while (getline(borrowersFile, line)) {
		size_t colonPos = line.find(FIRST_SEPARATOR);
		string reader = line.substr(0, colonPos);
		string borrowedBooks = line.substr(colonPos + 1);

		if (reader == readerName) {
			readerFound = true;

			vector<string> booksVec;
			stringstream ss(borrowedBooks);
			string book;
			while (getline(ss, book, SECOND_SEPARATOR)) {
				if (!book.empty()) {
					book = regex_replace(book, regex("^\\s+"), ""); // remove leading whitespace
					book = regex_replace(book, regex("\\s+$"), ""); // remove trailing whitespace
					if (book != bookName) {
						booksVec.push_back(book);
					}
				}
			}

			// Join the remaining books into a string
			string newBorrowedBooks = "";
			for (int i = 0; i < booksVec.size(); i++) {
				newBorrowedBooks += booksVec[i];
				if (i != booksVec.size() - 1) {
					newBorrowedBooks += SECOND_SEPARATOR;
				}
			}

			// If the borrower has no more borrowed books, skip writing their entry
			if (newBorrowedBooks.empty()) {
				continue;
			}

			tempBorrowersFile << reader << FIRST_SEPARATOR << newBorrowedBooks << endl;
		}
		else {
			tempBorrowersFile << line << endl;
		}
	}

	borrowersFile.close();
	tempBorrowersFile.close();

	if (!readerFound) {
		cout << "The reader was not found in the list of borrowers." << endl;
		remove("temp.txt");
		return;
	}

	remove(LIST_OF_BORROWERS_FILE.c_str());
	rename("temp.txt", LIST_OF_BORROWERS_FILE.c_str());
}


void Reader::borrowBook(string readerName) {
	cin.ignore();
	string bookName = safeStringInput();
	bool isBorrowed = false;
	bool bookFound = false;
	vector<Book> books = booksFileToVector();

	for (Book& book : books) {
		if (book.getTitle() == bookName) {
			bookFound = true;
			if (book.getAvailability()) {
				cout << "Sorry, the book is already borrowed." << endl;
			}
			else {
				book.setAvailability(true);
				cout << "Book borrowed successfully." << endl;
			}
		}
	}

		if (!bookFound) {
			cout << "Sorry, the book was not found." << endl;
			return;
		}
		
		vectorToFile(books);
	addBookToBorrower(readerName, bookName);
}

void Reader::returnBook(string readerName) {
	cin.ignore();
	string bookName = safeStringInput();
	bool bookFound = false;
	vector<Book> books = booksFileToVector();

	for (Book& book : books) {
		if (book.getTitle() == bookName) {
			bookFound = true;
			if (!book.getAvailability()) {
				cout << "The book is not borrowed now." << endl;
			}
			else {
				book.setAvailability(false); // set the borrowed status to false
				cout << "Book returned successfully." << endl;
			}
		}
	}

	if (!bookFound) {
		cout << "Sorry, the book was not found." << endl;
		remove("temp.txt");
		return;
	}

	vectorToFile(books);
	deleteBookFromBorrower(readerName, bookName);
}


void Reader::viewBorrowedBooks() {
	ifstream borrowersFile(LIST_OF_BORROWERS_FILE);
	if (!borrowersFile.is_open()) {
		std::cout << "Unable to open file." << endl;
		return;
	}

	string line;
	while (getline(borrowersFile, line)) {
		size_t colonPos = line.find(FIRST_SEPARATOR);
		string readerName = line.substr(0, colonPos);
		string borrowedBooks = line.substr(colonPos + 1);
		if (readerName != getLogin()) {
			continue;
		}

		cout << "+------------------------------------------------------+" << endl;
		cout << "             Books borrowed by " << setw(15) << left << readerName << "" << endl;
		cout << "+------------------------------------------------------+" << endl;
		stringstream ss(borrowedBooks);
		string bookName;
		while (getline(ss, bookName, SECOND_SEPARATOR)) {
			cout << "| " << setw(52) << left << bookName << "|" << endl;
		}
		cout << "+------------------------------------------------------+" << endl;
		return;
	}
	cout << "No record found for " << getLogin() << endl;
}

void readerLoginMenu(bool& exitMenu, Reader& reader)
{
	User user;
	std::system("cls");
	cout << "Welcome! Please choose an option:" << endl;
	cout << "1. Login" << endl;
	cout << "2. Create account" << endl;
	cout << "3. Exit" << endl;
	cout << "Enter choice: ";
	int choice;

	choice = user.safeIntInput();

	switch (choice) {
	case 1:
		reader.logInAccount(READER_ACCOUNTS_FILE);
		break;
	case 2:
		reader.createAccount(READER_ACCOUNTS_FILE);
		break;
	case 3:
		cout << "Exiting..." << endl;
		exitMenu = false;
		break;
	default:
		cout << "Invalid choice. Please try again." << endl;
		break;
	}

	std::system("pause");
}

void readerMenuSwitch(bool& exitMenu, Reader& reader, int choice)
{
	switch (choice) {
	case 1:
		reader.viewAllBooks();
		reader.borrowBook(reader.getLogin());
		break;
	case 2:
		reader.returnBook(reader.getLogin());
		break;
	case 3:
		reader.searchForABook();
		break;
	case 4:
		reader.sortBooks();
		break;
	case 5:
		reader.viewAllBooks();
		break;
	case 6:
		reader.viewBorrowedBooks();
		break;
	case 7:
		cout << "Logging out..." << endl;
		exitMenu = false;
		reader.logOut();
		break;
	default:
		cout << "Invalid choice. Please try again." << endl;
		break;
	}
}

void Reader::readerMenu(bool& exitMenu, Reader& reader)
{
	User user;
	if (reader.getLogged()) {

		std::system("cls");
		cout << "Please choose an option:" << endl;
		cout << "1. Borrow Book by title" << endl;
		cout << "2. Return Book by title" << endl;
		cout << "3. Search for a Book" << endl;
		cout << "4. Sort books" << endl;
		cout << "5. View All Books" << endl;
		cout << "6. View borrowed books" << endl;
		cout << "7. Logout" << endl;
		cout << "Enter choice: ";
		int choice;
		while (true) {
			cout << "Enter your choice (1-3): ";
			cin >> choice;

			if (cin.fail()) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Invalid choice. Please try again." << endl;
			}
			else {
				break;
			}
		}

		readerMenuSwitch(exitMenu, reader, choice);

		std::system("pause");
	}
	else {
		readerLoginMenu(exitMenu, reader);
	}
}
