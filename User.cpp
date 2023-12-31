#include <string>
#include <iostream>
#include <queue>
#include "User.h"
#include "Util.h"
#include "Customer.h"
#include "Employee.h"
#include "Manager.h"
#include "StringNode.h"


using namespace std;

void User::mainMenu() {

    string userChoice = "0";

    while (stoi(userChoice) != 4) {

        cout << "Welcome to Publix\n";
        cout << "------------------------------------\n";
        cout << "Would you like to:\n";
        cout << "1) Login\n";
        cout << "2) Register\n";
        cout << "3) Forgot Password\n";
        cout << "4) Quit\n";
        cout << "------------------------------------\n";

        userChoice = getNumberInRange(1, 4);

        if (stoi(userChoice) == 1) {
            if (verifyUser()) {
                logIn();
            }
        }

        else if (stoi(userChoice) == 2) {
            createUser(Role::customer);
            if (verifyUser()) {
                logIn();
            }
        }

        else if (stoi(userChoice) == 3) {
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
        username = getLine();

        if (username.compare("quit") == 0)
            return false;

        cout << "Password: ";
        password = getLineInvisible();

        if (!isValidPassword(password) || !isTakenUsername(username)) {
            cout << "Please enter a valid username or password\n";
            cout << "----------------------------------\n";
            continue;
        }


        salter = query(1, "SELECT salter FROM login WHERE username = '" + changeApostraphe(username) + "'").front();
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
        username = getLine();

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
        password = getLineInvisible();
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
    email = getEmail();

    cout << "Please enter the phone number you use: ";
    input = getPhoneNumber();

    phoneNumber = stoll(input);

    string query1 = "select customerID from customer where phoneNumber = " + to_string(phoneNumber) + " and emailAddress = '" + email + "'";
    result = query(1, query1);

    if (!result.empty()) {

        string userID = result.front();
        string salter = query(1, "select salter from login where userID = " + userID).front();

        cout << "New Password: ";
        input = getLineInvisible();

        while (!isValidPassword(input)) {
            cout << "Please enter a password with 8 characters, a number, uppercase letter and lowercase letter.\n";
            cout << "New Password: ";
            input = getLineInvisible();
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

string User::searchProducts() {

    string search;
    bool searching;
    int interestedProduct = 0, i;
    queue<string> result;
    string product;
    StringNode* head, * cursor;
    head = new StringNode();

    cout << "\nPlease enter the name of the product you're looking for. Type 'Done' to stop searching.\n";
    search = getLine();


    if (search.compare("Done") == 0 || search.compare("done") == 0)
        return "done";

    //main search query
    result = query(1, "Select productName from inventory where upper(productName) like upper('%" + changeApostraphe(search) + "%')");

    if (result.empty()) {
        cout << "No results\n";
        return "done";
    }

    //prints out all the product names that contain the search characters


    cursor = head;
    for (i = 1; !result.empty(); i++) {

        product = result.front();
        result.pop();
        cout << i << ") " + product + "\n";
        cout << "------------------------------\n";
        cursor->data = product;
        cursor->next = new StringNode(product);
        cursor = cursor->next;
    }

    //customer can look at a particular item more or search for new products 
    cout << "Enter the number for the product you want to look at more,the name of an item you want to search for, or 'done' to stop searching.\n";
    search = getLine();

    try {
        interestedProduct = stoi(search);

        //if the number wasn't listed, startes a new search
        if (interestedProduct > i || interestedProduct < 1) {
            cout << "Number not in acceptable range.\n";
            return "done";
        }
    }
    catch (...) {
        return "done";
    }

    cursor = head;
    for (int i = 1; i < interestedProduct; i++) {
        cursor = cursor->next;
    }

    return query(1, "select productID from inventory where productName = '" + changeApostraphe(cursor->data) + "'").front();
}

void User::changePasswordLoggedIn() {

    string input;
    cout << "Please enter the new password: ";
    input = getLineInvisible();

    while (!isValidPassword(input)) {
        cout << "Please enter a password 8 characters long, with an uppercase letter, lowercase letter and a number.\n";
        cout << "New password: ";
        input = getLineInvisible();
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