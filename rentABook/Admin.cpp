#include "User.h";
#include "Account.h"
#include "Book.h";
#include "Admin.h";

#include <conio.h>
#include <regex>
#include <sstream>
#include <iomanip>
#include <random>

using namespace std;

const string LIST_OF_BOOKS_FILE = "list_of_books.txt";
const string LIST_OF_BORROWERS_FILE = "list_of_borrowers.txt";
const string VALID_INPUT_INT = "int";
const int SALT_LENGTH = 8;


void aadminMenu(Admin& admin) {
	string fileName = "admin_account.txt";
	int choice;

	bool exitMenu = true;
	while (exitMenu) {
		if (admin.getLogged()) {

			std::system("cls");
			system("color F0");
			cout << "\nWelcome Admin!" << endl;
			cout << "Please choose an option:" << endl;
			cout << "1. Add Book" << endl;
			cout << "2. Delete Book" << endl;
			cout << "3. Search for a Book" << endl;
			cout << "4. Sort books" << endl;
			cout << "5. View All Books" << endl;
			cout << "6. View All Readers" << endl;
			cout << "7. View All Borrowers" << endl;
			cout << "8. Log out" << endl;



			while (true) {
				cout << "Enter your choice (1-8): ";
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

			cin.ignore();

			switch (choice) {
			case 1:
				admin.addBook();
				break;
			case 2:
				admin.deleteBook();
				break;
			case 3:
				admin.searchForABook();
				break;
			case 4:
				admin.sortBooks();
				break;
			case 5:
				admin.viewAllBooks();
				break;
			case 6:
				admin.viewAllReaders();
				break;
			case 7:
				admin.viewAllBorrowers();
				break;
			case 8:
				cout << "Logging out..." << endl;
				system("color 0F");
				admin.logOut();
				break;
			default:
				cout << "Invalid choice. Please try again." << endl;
				break;
			}

			std::system("pause");
		}
		else {
			std::system("cls");
			cout << "Welcome! Please choose an option:" << endl;
			cout << "1. Login" << endl;
			cout << "2. Create account" << endl;
			cout << "3. Exit" << endl;
			cout << "Enter choice: ";

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
			switch (choice) {
			case 1:
				admin.logInAccount(fileName);
				break;
			case 2:
				admin.createAccount(fileName);
				break;
			case 3:
				cout << "Exiting..." << endl;
				//return exitMenu = false;
				//main();
				break;
			default:
				cout << "Invalid choice. Please try again." << endl;
				break;
			}
			std::system("pause");
		}

	}
}


void Admin::addBook() {
	cin.ignore();
	Book book;
	ofstream bookFile(LIST_OF_BOOKS_FILE, ios::app);
	if (bookFile.is_open()) {
		string str;
		if (book.getAvailability()) str = "1";
		else str = "0";
		bookFile << book.getTitle() << "," << book.getAuthor() << "," << book.getYear() << "," << str << "," << book.getISBN() << endl;
		bookFile.close();
		cout << "Book added successfully!" << endl;
	}
	else {
		cout << "Unable to open file." << endl;
	}
}

void deleteBookFromBorrowers(string bookTitle)
{
	ifstream borrowersFile(LIST_OF_BORROWERS_FILE);
	ofstream tempBorrowersFile("temp_borrowers.txt", ios::app);
	string borrowersLine;

	if (borrowersFile.is_open() && tempBorrowersFile.is_open()) {
		bool found = false;
		while (getline(borrowersFile, borrowersLine)) {
			size_t bookTitlePos = borrowersLine.find(bookTitle);
			if (bookTitlePos != string::npos) {
				found = true;
				size_t delimiterPos = borrowersLine.find(":");
				string borrowerId = borrowersLine.substr(0, delimiterPos);
				string books = borrowersLine.substr(delimiterPos + 1);
				size_t bookPos = books.find(bookTitle);
				books.erase(bookPos, bookTitle.length());
				// Remove any extra commas or spaces
				books.erase(0, books.find_first_not_of(", "));
				books.erase(books.find_last_not_of(", ") + 1);
				tempBorrowersFile << borrowerId << ":" << books << endl;
			}
			else {
				tempBorrowersFile << borrowersLine << endl;
			}
		}
		borrowersFile.close();
		tempBorrowersFile.close();

		if (!found) {
			cout << "The book could not be found in the list of borrowers." << endl;
			remove("temp_borrowers.txt");
		}
		else {
			remove("list_of_borrowers.txt");
			rename("temp_borrowers.txt", "list_of_borrowers.txt");
		}
	}
	else {
		cout << "Unable to open file." << endl;
	}
}

void Admin::deleteBook() {
	string bookTitle = safeStringInput();
	ifstream bookFile(LIST_OF_BOOKS_FILE);
	ofstream tempFile("temp.txt", ios::app);
	string line;

	vector<Book> books = booksFileToVector();

	bool found = false;

	for (Book& book : books) {
		if (book.getTitle() == bookTitle) {
			found = true; 
			books.erase(remove_if(books.begin(), books.end(),
				[bookTitle](const Book& b) { return b.getTitle() == bookTitle; }), books.end());

			cout << "the book has been removed" << endl;
		}
		else {
			tempFile << line << endl;
		}
	}

	if (!found) {
		cout << "The book could not be found." << endl;
		remove("temp.txt");
		return;
	}
	vectorToFile(books);

	deleteBookFromBorrowers(bookTitle);
}





void Admin::viewAllReaders() {
	ifstream readerFile(READER_ACCOUNTS_FILE);
	string line;

	if (readerFile.is_open()) {
		std::system("cls");
		cout << left << setw(40) << setfill('=') << "" << endl;
		cout << setfill(' ');
		cout << setw(35) << left << "|Reader login" << "|" << endl;
		cout << setw(40) << setfill('=') << "" << endl;
		while (getline(readerFile, line)) {
			stringstream ss(line);
			string login;
			getline(ss, login, ',');
			cout << setw(35) << left << "|" + login << "|" << endl;
		}
		cout << setw(40) << setfill('=') << "" << endl;
		readerFile.close();
	}
	else {
		cout << "Unable to open file." << endl;
	}
}
void Admin::viewAllBorrowers() {
	ifstream borrowersFile(LIST_OF_BORROWERS_FILE);
	string line;

	if (borrowersFile.is_open()) {
		std::system("cls");
		cout << left << setw(60) << setfill('=') << "" << endl;
		cout << setfill(' ');
		cout << setw(30) << left << "|Borrower" << "|Books Borrowed" << endl;
		cout << setw(60) << setfill('=') << "" << endl;
		while (getline(borrowersFile, line)) {
			stringstream ss(line);
			string borrowerName, bookName;
			getline(ss, borrowerName, ':');

			cout << setw(30) << left << "|" + borrowerName;

			// Read the first book name
			getline(ss, bookName, ',');
			cout << "|" + bookName;

			// Read and print the remaining book names
			while (getline(ss, bookName, ',')) {
				cout << ", " << bookName;
			}
			cout << endl;
		}
		cout << setw(60) << setfill('=') << "" << endl;
		borrowersFile.close();
	}
	else {
		cout << "Unable to open file." << endl;
	}
}

void adminLoginMenu(bool& exitMenu, Admin& admin, int choice)
{
	cout << "Welcome! Please choose an option:" << endl;
	cout << "1. Login" << endl;
	cout << "2. Create account" << endl;
	cout << "3. Exit" << endl;
	cout << "Enter choice: ";

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
	switch (choice) {
	case 1:
		admin.logInAccount(ADMIN_ACCOUNTS_FILE);
		break;
	case 2:
		admin.createAccount(ADMIN_ACCOUNTS_FILE);
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

void adminSwitchCase(Admin& admin, int choice)
{

	switch (choice) {
	case 1:
		admin.addBook();
		break;
	case 2:
		admin.deleteBook();
		break;
	case 3:
		admin.searchForABook();
		break;
	case 4:
		admin.sortBooks();
		break;
	case 5:
		admin.viewAllBooks();
		break;
	case 6:
		admin.viewAllReaders();
		break;
	case 7:
		admin.viewAllBorrowers();
		break;
	case 8:
		cout << "Logging out..." << endl;
		system("color 0F");
		admin.logOut();
		break;
	default:
		cout << "Invalid choice. Please try again." << endl;
		break;
	}
}

void Admin::adminMenu(bool& exitMenu, Admin& admin)
{
	User user;
	int choice = 0;
	if (admin.getLogged()) {

		std::system("cls");
		system("color F0");
		cout << "\nWelcome Admin!" << endl;
		cout << "Please choose an option:" << endl;
		cout << "1. Add Book" << endl;
		cout << "2. Delete Book By title" << endl;
		cout << "3. Search for a Book" << endl;
		cout << "4. Sort books" << endl;
		cout << "5. View All Books" << endl;
		cout << "6. View All Readers" << endl;
		cout << "7. View All Borrowers" << endl;
		cout << "8. Log out" << endl;

		choice = safeIntInput();
		cin.ignore();
		adminSwitchCase(admin, choice);

		std::system("pause");
	}
	else {
		std::system("cls");
		adminLoginMenu(exitMenu, admin, choice);
	}

}