#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>



using namespace std;


class Book {
private:
    string title;
    string author;
    int year;
    bool isAvailable;
    string currentBuyer;
    string isbn;
public:
    bool operator<(const Book& other) const;
    string getTitle() const;
    void setTitle(string title);
    string getAuthor() const;
    void setAuthor(string author);
    int getYear() const;
    void setYear(int year);
    bool getAvailability();
    void setAvailability(bool availability);
    string getISBN() const;
    void setISBN(string isbn);
    string validInput(string type);
    Book();
    Book(string t, string a, int y, bool b, string i);
};