
#include <iostream>
#include <string>

#include "Book.h"
#include "User.h"
#include "Account.h"
#include "Admin.h"
#include "Reader.h"

using namespace std;
const string VALID_INPUT_INT = "int";


int main()
{
    int choice;
    Admin admin;
    Reader reader;
    User user;

    bool exitMenu = true;
    while (true) {
        std::system("cls");
        cout << "Welcome to the Library!" << endl;
        cout << "Please choose an option:" << endl;
        cout << "1. Admin" << endl;
        cout << "2. Reader" << endl;
        cout << "3. Guest" << endl;
        cout << "4. Exit" << endl;
        
        choice = user.safeIntInput();

        cin.ignore();

        switch (choice) {
        case 1:
            exitMenu = true;
			while (exitMenu) {
                admin.adminMenu(exitMenu, admin);
			}
            break;
        case 2:
            exitMenu = true;
			while (exitMenu) {
				reader.readerMenu(exitMenu, reader);
			}
                break;
        case 3:
            user.guestMenu();
            break;
        case 4:
            cout << "Goodbye!" << endl;
            exitMenu = false;
            admin.logOut();
            reader.logOut();
            return 0;
        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
        }
    }
    return 0;
}