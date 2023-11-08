#include "User.h";
#include "Book.h";

#include <conio.h>
#include <regex>
#include <sstream>
#include <iomanip>
#include <random>
#include <iostream>
#include <vector>

const string LIST_OF_BOOKS_FILE = "list_of_books.txt";
const string LIST_OF_BORROWERS_FILE = "list_of_borrowers.txt";
const int SALT_LENGTH = 8;


const string VALID_INPUT_TITLE = "title";
const string VALID_INPUT_INT = "int";
const string VALID_INPUT_AUTHOR = "author";
const string VALID_INPUT_YEAR = "year";
const string VALID_INPUT_ISBN = "isbn";
const string VALID_INPUT_AVAILABILITY = "available";

const string TITLE_PATTERN = "^[a-zA-Z0-9 ]+$"; 
const string INT_PATTERN = "^\\d{4}$";


const int TITLE_WIDTH = 40;
const int AUTHOR_WIDTH = 25;
const int YEAR_WIDTH = 10;
const int ISBN_WIDTH = 20;
const int AVAILABILITY_WIDTH = 15;

using namespace std;

vector<Book> booksFileToVector();

void printTableHeader() {
	cout << "===========================================================================================================================" << endl;
	cout << "| Title                                    | Author                    | Year       | ISBN                 | Availability |" << endl;
	cout << "===========================================================================================================================" << endl;
}

void printRow(Book book) {

	cout << setfill(' ');
	cout << "| " << setw(TITLE_WIDTH) << left << book.getTitle() << " | "
		<< setw(AUTHOR_WIDTH) << left << book.getAuthor() << " | "
		<< setw(YEAR_WIDTH) << right << book.getYear() << " | "
		<< setw(ISBN_WIDTH) << left << book.getISBN() << " | "
		<< setw(AVAILABILITY_WIDTH) << left << (book.getAvailability() ? "Yes" : "No") << " |" << endl;
}


void User::searchForABook() {
	cin.ignore();
	string bookName = safeStringInput();
	vector<Book> books = booksFileToVector();
	bool bookFound = false;
	for (Book& book : books) {
		if (book.getTitle() == bookName) {
			bookFound = true;
			printTableHeader();
			printRow(book);
		}
	}
	if (!bookFound) {
		cout << "Sorry, the book was not found." << endl;
	}
}

string User::safeStringInput()
{
	regex pattern;
	string input;
	pattern = regex(TITLE_PATTERN);
	cout << "Enter string: ";
	getline(cin, input);

	while (!regex_match(input, pattern)) {
		cout << "Invalid input. Enter string again ";
		getline(cin, input);
	}
	return input;
}

int User::safeIntInput()
{
	regex pattern;
	int input;
	pattern = regex(INT_PATTERN);
	cout << "Enter int: ";
	cin >> input;

	while (cin.fail() || input > 2023 || input < 0) {
		cout << "Invalid input. Enter int again ";
		cin >> input;
	}
	return input;
}

void User::printBooks(vector<Book>& books) {
	printTableHeader();

	cout << setfill('-') << setw(TITLE_WIDTH + AUTHOR_WIDTH + YEAR_WIDTH + ISBN_WIDTH + AVAILABILITY_WIDTH + 10) << "" << endl;

	for (Book& book : books) {
		printRow(book);
	}
	cout << setfill('-') << setw(TITLE_WIDTH + AUTHOR_WIDTH + YEAR_WIDTH + ISBN_WIDTH + AVAILABILITY_WIDTH + 10) << "" << endl << endl;;
}

void User::viewAllBooks() {
	vector<Book> books = booksFileToVector();
	printBooks(books);
}

void User::guestMenu() {
	int choice;
	User user;
	do {
		std::system("cls");
		cout << "===============================" << endl;
		cout << " Welcome to Guest Menu" << endl;
		cout << "===============================" << endl;
		cout << "1. View all books" << endl;
		cout << "2. Search for a book" << endl;
		cout << "3. Sort books" << endl;
		cout << "4. Exit" << endl;
		cout << "===============================" << endl;

		choice = safeIntInput();

		switch (choice) {
		case 1:
			user.viewAllBooks();
			break;
		case 2:
			user.searchForABook();
			break;
		case 3:
			user.sortBooks();
			break;
		case 4:
			cout << "Exiting Guest Menu..." << endl;
			break;
		default:
			cout << "Invalid choice. Please try again." << endl;
			break;
		}

		std::system("pause");
	} while (choice != 4);
}


void User::vectorToFile(vector<Book> books) {
	ofstream bookFile(LIST_OF_BOOKS_FILE);
	if (bookFile.is_open()) {
		for (Book& book : books) {
			bookFile << book.getTitle() << ","
				<< book.getAuthor() << ","
				<< book.getYear() << ","
				<< (book.getAvailability() ? "1" : "0") << ","
				<< book.getISBN() << endl;
		}
		bookFile.close();
	}
	else {
		cout << "Unable to open file." << endl;
	}
}
vector<Book> User::booksFileToVector()
{
	ifstream bookFile(LIST_OF_BOOKS_FILE);
	string title, author, year, isbn;
	bool isAvailable;
	vector<Book> books;

	if (bookFile.is_open()) {
		while (getline(bookFile, title, ',') && getline(bookFile >> ws, author, ',') && getline(bookFile, year, ',') && (bookFile >> isAvailable) && bookFile.ignore() && getline(bookFile, isbn)) {
			
			isbn.erase(isbn.find_last_not_of(" \n\r\t") + 1);

			Book book(title, author, stoi(year), isAvailable, isbn);
			books.push_back(book);
		}
		bookFile.close();
	}
	return books;
}



void User::sortBooks() {
	vector<Book> books = booksFileToVector();
	vector<Book> filteredBooks;
	int inputYear;

	cout << "Sort by:" << endl << "1. Author" << endl << "2. Title" << endl << "3. Year" << endl << "4. Not available" << endl << "5. Year after input" << endl;
	int sortBy = safeIntInput();

	switch (sortBy) {
	case 1:
		SortBooksByCriteria(books, VALID_INPUT_AUTHOR);
		printBooks(books);
		break;
	case 2:
		SortBooksByCriteria(books, VALID_INPUT_TITLE);
		printBooks(books);
		break;
	case 3:
		SortBooksByCriteria(books, VALID_INPUT_YEAR);
		printBooks(books);
		break;
	case 4:
		SortBooksByCriteria(books, VALID_INPUT_AVAILABILITY);
		printBooks(books);
		break;
	case 5:
		inputYear = safeIntInput();

		for (const Book& book : books) {
			if (book.getYear() > inputYear) {
				filteredBooks.push_back(book);
			}
		}

		SortBooksByCriteria(filteredBooks, VALID_INPUT_AUTHOR);
		printTableHeader();
		for (Book& book : filteredBooks) {
			printRow(book);
		}
		break;
	default:
		cout << "Invalid input." << endl;
		break;

	}
}

void User::SortBooksByCriteria(vector<Book>& books, string criteria)
{
	if (criteria == VALID_INPUT_AVAILABILITY)
		std::sort(books[i], books[j]);
	for (int i = 0; i < books.size() - 1; i++) {
		for (int j = i + 1; j < books.size(); j++) {
			if ((criteria == VALID_INPUT_AUTHOR && books[i].getAuthor() > books[j].getAuthor()) ||
				(criteria == VALID_INPUT_TITLE && books[i].getTitle() > books[j].getTitle()) ||
				(criteria == VALID_INPUT_AVAILABILITY && books[i].getAvailability() > books[j].getAvailability()) ||
				//(criteria == VALID_INPUT_YEAR && books[i].getYear() > books[j].getYear())
				) {
				swap(books[i], books[j]);
			}
		}
	}
}