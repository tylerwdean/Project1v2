#include <string>
#include <iostream>
#include <queue>
#include "User.h"
#include "Util.h"
#include "Customer.h"
#include "Employee.h"
#include "Manager.h"


using namespace std;

void User::mainMenu() {

    int userChoice = 0;

    while (userChoice != 4) {

        cout << "Welcome to Publix\n";
        cout << "------------------------------------\n";
        cout << "Would you like to:\n";
        cout << "1) Login\n";
        cout << "2) Register\n";
        cout << "3) Forgot Password\n";
        cout << "4) Quit\n";
        cout << "------------------------------------\n";

        cin >> userChoice;

        while (userChoice < 1 || userChoice > 4) {
            cout << "Please try entering again:\n";
            cin >> userChoice;
        }

        if (userChoice == 1) {
            if (verifyUser()) {
                logIn();
            }
        }

        else if (userChoice == 2) {
            createUser(Role::customer);
            if (verifyUser()) {
                logIn();
            }
        }

        else if (userChoice == 3) {
            changePassword();
        }

    }

    return;


}


bool User::verifyUser() {

    string username;
    string password;
    string salter;
    string hash;
    bool isVerified = false;

    while (!isVerified) {

        cout << "Username: ";
        cin >> username;

        if (username.compare("quit") == 0)
            return false;

        cout << "Password: ";
        cin >> password;

        //ensures that endered password is clean
        if (!isValidPassword(password) || !isTakenUsername(username)) {
            cout << "Please enter a valid username\n";
            cout << "----------------------------------\n";
            continue;
        }


        salter = query(1, "SELECT salter FROM login WHERE username = '" + username + "'").front();
        hash = hashString(password, salter);
        queue<string> result = query(1, "SELECT userID FROM login WHERE password = '" + hash + "'");

        if (!result.empty()) {
            string userID = result.front();
            this->userID = std::stoi(userID);
            isVerified = true;
        }

        //if the query was nothing then we aren't logged in and so it tries again
        else {
            cout << "Verification Failed, Please try again\n";
            cout << "------------------------------------\n";
            isVerified = false;
        }
        
    }

    return isVerified;

}


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

        else if (isTakenUsername(username)) {
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
        if (isValidPassword(password)) {
            break;
        }

        cout << "That's not a valid password. Please have at least 8 characters, \n an uppercase letter, a lowercase letter and a number." << endl;
        cout << "Enter new password: ";

    }

    //generates the salter for the hash, hashes and stores the info in the database
    salter = generateRandomString(16);
    hash = hashString(password, salter);
    query(0, "INSERT INTO login values ('" + hash + "', '" + salter + "', '" + username + "')");
    string userID = query(1, "select userID from login where password = '" + hash + "'").front();
    query(0, "insert into roles values (" + userID + ", 'customer')");
    cout << "Account created! Please login now." << endl;
    cout << "------------------------------------\n";

    return;
}

void User::logIn() {

    string roleString = query(1, "SELECT role FROM roles WHERE userID = " + to_string(this->userID)).front();
    if (roleString.compare("employee") == 0) {
        Employee currentUser = Employee(userID);
        currentUser.mainMenu();
    }

    else if (roleString.compare("manager") == 0) {
        Manager currentUser = Manager(userID);
        currentUser.mainMenu();
    }

    else {
        Customer currentUser = Customer(userID);
        currentUser.mainMenu();
    }

    return;

}

void User::changePassword() {

    string input;
    string email;
    long long phoneNumber;
    queue<string> result;

    cout << "Please enter the email address you use: ";
    cin >> input; 

    while (!isValidEmail(input)) {

        if (input.compare("quit") == 0)
            return;

        cout << "Please enter valid email address: ";
        cin >> input;
    }

    email = input;

    cout << "Please enter the phone number you use: ";
    cin >> input;

    while (!isValidPhoneNumber(input)) {
        cout << "Please enter valid phone number, digits only: ";
        cin >> input;
    }

    phoneNumber = stoll(input);

    string query1 = "select customerID from customer where phoneNumber = " + to_string(phoneNumber) + " and emailAddress = '" + email + "'";
    result = query(1, query1);

    if (!result.empty()) {

        string userID = result.front();
        string salter = query(1, "select salter from login where userID = " + userID).front();

        cout << "New Password: ";
        cin >> input;

        while (!isValidPassword(input)) {
            cout << "Please enter a password with 8 characters, a number, uppercase letter and lowercase letter.\n";
            cout << "New Password: ";
            cin >> input;
        }

        string newPassword = hashString(input, salter);
        string query2 = "update login set password = '" + newPassword + "' where userID = " + userID;
        query(0, query2);
        cout << "Password is updated. You can login with that password now.\n";
    }

    else {
        cout << "Could not verify account. Please login to change password.\n";
    }

    return;
}

void User::changePasswordLoggedIn() {

    string input;
    cout << "Please enter the new password: ";
    cin >> input;

    while (!isValidPassword(input)) {
        cout << "Please enter a password 8 characters long, with an uppercase letter, lowercase letter and a number.\n";
        cout << "New password: ";
        cin >> input;
    }

    string newPassword = input;
    string salter = query(1, "select salter from login where userID = " + to_string(userID)).front();
    string hash = hashString(newPassword, salter);
    string query0 = "update login set password = '" + hash + "' where userID = " + to_string(this->userID);
    query(0, query0);
}


void User::viewInformation() {

    string firstName, lastName, socialSecurity, phoneNumber;
    queue<string> result;

    if (userRole == customer) {

        string query0 = "select firstName, lastName, phoneNumber, emailAddress from customer where customerID = " + to_string(userID);
        result = query(4, query0);
        cout << "First name: ";
        cout << result.front();
        result.pop();
        cout << "\nLast name: ";
        cout << result.front();
        result.pop();
        cout << "\nPhone Number: ";
        cout << result.front();
        result.pop();
        cout << "Email Address: ";
        cout << result.front();
        result.pop();
        cout << endl;

        return;
    }

    else if (userRole == employee || userRole == manager) {

        string query0 = "select firstName, lastName, phoneNumber from employee where employeeID = " + to_string(userID);
        result = query(3, query0);
        cout << "First name: ";
        cout << result.front();
        result.pop();
        cout << "\nLast name: ";
        cout << result.front();
        result.pop();
        cout << "\nPhone Number: ";
        cout << result.front();
        result.pop();
        cout << endl;

        return;
    }


}