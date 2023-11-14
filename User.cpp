#include <string>
#include <iostream>
#include "User.h"
#include "Util.h"
#include "Customer.h"
#include "Employee.h"
#include "Manager.h"


using namespace std;

void User::mainMenu() {

    int userChoice = 0;

    while (userChoice != 3) {

        cout << "Welcome to Publix's POS\n";
        cout << "------------------------------------\n";
        cout << "Would you like to:\n";
        cout << "1) Login\n";
        cout << "2) Register\n";
        cout << "3) Quit\n";
        cout << "------------------------------------\n";

        cin >> userChoice;

        while (userChoice < 1 || userChoice > 3) {
            cout << "Please try entering again:\n";
            cin >> userChoice;
        }

        if (userChoice == 1) {
            verifyUser();
            loggedIn();
        }

        else if (userChoice == 2) {
            createUser(Role::customer);
            verifyUser();
            loggedIn();
        }

    }

    return;


}


void User::verifyUser() {

    string username;
    string password;
    string salter;
    string hash;
    bool isVerified = false;

    while (!isVerified) {

        cout << "Username: ";
        cin >> username;

        if (username.compare("quit") == 0)
            return;

        cout << "Password: ";
        cin >> password;

        //ensures that endered password is clean
        if (!isValid(password) || !isTaken(username)) {
            cout << "Verification Failed, Please try again\n";
            cout << "------------------------------------\n";
            verifyUser();
        }


        salter = "ABCDEF";//query(1, "SELECT salter FROM login WHERE username = \'" + username + "\'").front();
        hash = hashString(password, salter);
        this->userID = 1;//std::stoi(query(1, "SELECT ID FROM login WHERE password = \'" + hash + "\'").front());

        //if the query was null then we aren't logged in and so it tries again
        if (this->userID == NULL) {
            cout << "Verification Failed, Please try again\n";
            cout << "------------------------------------\n";
            verifyUser();
        }

        else {
            isVerified = true;
        }

    }

    cout << "------------------------------------\n";

}


//this function takes a username, password, creates a salter and stores the information in the database
void User::createUser(Role userRole) {

    string username;
    string password = "";
    string salter;
    string hash;

    cout << "Enter new Username: ";

    //verifies the username to ensure it's clean and not taken
    while (true) {
        cin >> username;

        if (!isClean(username)) {
            cout << "Don't try to break my code lmao, try again" << endl;
        }

        else if (isTaken(username)) {
            cout << "That username is taken, please try again" << endl;
        }

        else {
            break;
        }

        cout << "Enter new Username: ";
    }

    cout << "Enter new password: ";

    //verifies the password meets the requirements
    while (true) {
        cin >> password;
        if (isValid(password)) {
            break;
        }

        cout << "That's not a valid password. Please have at least 8 characters, \n an uppercase letter, a lowercase letter and a number." << endl;
        cout << "Enter new password: ";

    }

    //generates the salter for the hash, hashes and stores the info in the database
    salter = generateRandomString(16);
    hash = hashString(password, salter);
    //query("INSERT INTO login values (\"" + username + "\", \"" + salter + "\", \"" + hash + "\")");
    cout << "Account created! Please login now." << endl;
    cout << "------------------------------------\n";

    return;
}

void User::loggedIn() {

    User currentUser;
    string roleString = "customer";
    //query(1, "SELECT role FROM Role WHERE ID = " + userID);

    if (roleString.compare("employee") == 0) {
        //currentUser = Employee(userID);
    }

    else if (roleString.compare("manager") == 0) {
        //currentUser = Manager(userID);
    }

    else {
        //currentUser = Customer(userID);
    }

    currentUser.mainMenu();
    delete &currentUser;

}