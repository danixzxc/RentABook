#include "Book.h"
#include <regex>
using namespace std;


const string VALID_INPUT_TITLE = "title";
const string VALID_INPUT_AUTHOR = "author";
const string VALID_INPUT_YEAR = "year";
const string VALID_INPUT_ISBN = "isbn";
const string VALID_INPUT_BORROWED = "borrowed";
const string YEAR_PATTERN = "^\\d{4}$";
const string TITLE_PATTERN = "^[a-zA-Z0-9 ]+$";
const string AUTHOR_PATTERN = "^[a-zA-Z ]+$";
const string ISBN_PATTERN = "^\\d{3}-\\d-\\d{5}-\\d{3}-\\d$";
const string BOUGHT_PATTERN = "^[01]$";

string Book::getTitle() const {
    return title;
}

void Book::setTitle(string title) {
    this->title = title;
}

string Book::getAuthor() const {
    return author;
}

void Book::setAuthor(string author) {
    this->author = author;
}

int Book::getYear() const {
    return year;
}

void Book::setYear(int year) {
    this->year = year;
}

bool Book::getAvailability() {
    return isAvailable;
}

void Book::setAvailability(bool availability) {
    isAvailable = availability;
}


string Book::getISBN() const {
    return isbn;
}

void Book::setISBN(string isbn) {
    this->isbn = isbn;
}

Book::Book() {
    cout << "Please enter the following information about the book:" << endl;
    title = validInput(VALID_INPUT_TITLE);
    author = validInput(VALID_INPUT_AUTHOR);
    isbn = validInput(VALID_INPUT_ISBN);
    year = stoi(validInput(VALID_INPUT_YEAR));
    int a;
    a = stoi(validInput(VALID_INPUT_BORROWED));
    if (a == 1)isAvailable = true;
    else if (a == 0)isAvailable = false;
}

bool Book::operator<(const Book& other) const {
    return year < other.year;
}

Book::Book(string t, string a, int y, bool b, string i) {
    title = t;
    author = a;
    year = y;
    isAvailable = b;
    isbn = i;
}

string Book::validInput(string type) {
    string input;
    regex pattern;
    if (type == VALID_INPUT_YEAR) {
        pattern = regex(YEAR_PATTERN);
        cout << "Enter year: ";
    }
    else if (type == VALID_INPUT_TITLE) {
        pattern = regex(TITLE_PATTERN);
        cout << "Enter title: ";
    }
    else if (type == VALID_INPUT_AUTHOR) {
        pattern = regex(AUTHOR_PATTERN);
        cout << "Enter author: ";
    }
    else if (type == VALID_INPUT_ISBN) {
        pattern = regex(ISBN_PATTERN);
        cout << "Enter ISBN (format: ###-#-#####-###-#): ";
    }
    else if (type == VALID_INPUT_BORROWED) {
        pattern = regex(BOUGHT_PATTERN);
        cout << "Enter 0 for not borrowed or 1 for borrowed: ";
    }
    else {
        cout << "Invalid type" << endl;
        return "";
    }
    getline(cin, input);
    while (!regex_match(input, pattern)) {
        cout << "Invalid input. ";
        if (type == VALID_INPUT_YEAR) {
            cout << "Enter year (positive integer): ";
        }
        else if (type == VALID_INPUT_TITLE) {
            cout << "Enter title (a possible title, without random symbols): ";
        }
        else if (type == VALID_INPUT_AUTHOR) {
            cout << "Enter author (only surname or full name, without symbols): ";
        }
        else if (type == VALID_INPUT_ISBN) {
            cout << "Enter ISBN (format: ###-#-#####-###-#): ";
        }
        else if (type == VALID_INPUT_BORROWED) {
            cout << "Enter 0 for not borrowed or 1 for borrowed: ";
        }
        getline(cin, input);
    }
    return input;
}
