#include "Account.h";
#include "Book.h";

#include <conio.h>
#include <regex>
#include <sstream>
#include <iomanip>
#include <random>

const string VALID_INPUT_LOGIN = "login";
const string VALID_INPUT_PASSWORD = "password";
const string PASSWORD_PATTERN = "^[a-zA-Z0-9!@#$%^&*()_+-=,.<>?;:'\"/\\\\\\[\\]{}|`~]+$";
const string LOGIN_PATTERN = "^[a-zA-Z0-9_-]+$";
const string SEPARATOR_CHARACTER = ",";
const int SALT_LENGTH = 8;

string GenerateSalt(int _length) {
    srand(time(NULL));
    string salt = "";
    for (int i = 0; i < _length; i++) salt += (char)(rand() % 26) + 'a';
    return salt;
}

size_t GenerateHash(string password, string salt) {
    password += salt;
    srand(time(NULL));
    char pepper = (char)(rand() % 26) + 'a';
    password += pepper;
    return hash<string>{}(password);
}

bool CompareHash(string password, string salt, size_t _hash) {
    password += salt;
    for (char i = 'a'; i <= 'z'; i++) {
        if (hash<string>{}(password + i) == _hash) return true;
    }
    return false;
}


string Account::validInput(string type) {
    //cin.ignore();
    string input;
    regex pattern;
    if (type == VALID_INPUT_LOGIN) {

        pattern = regex(LOGIN_PATTERN);
        cout << "Enter login: ";
    }
    else if (type == VALID_INPUT_PASSWORD) {
        pattern = regex(PASSWORD_PATTERN);
        cout << "Enter password: ";
    }
    else {
        cout << "Invalid type" << endl;
        return "";
    }
    getline(cin, input);
    while (!regex_match(input, pattern)) {
        cout << "Invalid input. ";
        system("cls");
        if (type == VALID_INPUT_LOGIN) {
            cout << "Enter login: ";
        }
        else if (type == VALID_INPUT_PASSWORD) {
            cout << "Enter password: ";
        }
        getline(cin, input);
    }
    return input;
}

void Account::createAccount(string fileName)
{
    ofstream out;
    out.open(fileName, ios::app);
    if (out.is_open())
    {
        login = validInput(VALID_INPUT_LOGIN);
        password = validInput(VALID_INPUT_PASSWORD);

        salt = GenerateSalt(SALT_LENGTH);
        hashedPassword = GenerateHash(password, salt);


        out << login << SEPARATOR_CHARACTER << hashedPassword << SEPARATOR_CHARACTER << salt << endl;
    }
    out.close();
}

void Account::logOut()
{
    isLogged = false;
}

string Account::getLogin()
{
    return login;
}

void Account::logInAccount(string fileName) {
    ifstream out;
    out.open(fileName); 
    if (out.is_open()) {
        login = validInput(VALID_INPUT_LOGIN);
        cout << "Enter password:" << endl;
        password = "";
        char c;
        while ((c = _getch()) != '\r') {
            if (c == '\b') { 
                if (!password.empty()) {
                    password.pop_back();
                    cout << "\b \b";
                }
            }
            else {
                password += c;
                cout << "*";
            }
        }

        loginCheck(fileName, salt, password, login);
    }
    out.close();
}

void Account::loginCheck(string fileName, string salt, string password, string login)
{
    ifstream out;
    out.open(fileName);
    if (out.is_open()) {
        string originalLogin, originalPassword;
        bool isFound = false;
        while (getline(out, originalLogin, ',') && getline(out, originalPassword, ',') && getline(out, salt)) {
            size_t hashValue;
            std::istringstream(originalPassword) >> hashValue;

            if (login == originalLogin && CompareHash(password, salt, hashValue)) {
                cout << "\nYou logged in successfully!" << endl;
                isLogged = true;
                isFound = true;
                break;
            }
        }
        if (!isFound) {
            cout << "\nInvalid login or password!" << endl;
        }
        out.close();
    }
}

bool Account::getLogged()
{
    return isLogged;
}