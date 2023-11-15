#include <string>
#include "Util.h"
#include "Customer.h"

//initializes the user and updates the first name for menu purposes
Customer::Customer(int userID) {
    this->userID = userID;
    queue<string> firstName = query(1, "select firstName from customer where customerID = " + to_string(userID));
    
    if (!firstName.empty()) {
        this->userFirstName = firstName.front();
    }

    else {
        this->userFirstName = query(1, "select username from login where userID = " + to_string(userID)).front();
    }

    mainMenu();
}

//main customer options
void Customer::mainMenu() {

    int userChoice = 0;

    while (userChoice != 7) {

        cout << "\nHello " + this->userFirstName + "!\n";
        cout << "What would you like to do?\n";
        cout << "1) Search products\n";
        cout << "2) See current cart\n";
        cout << "3) Check points balance\n";
        cout << "4) Update your information\n";
        cout << "5) Change password\n";
        cout << "6) See order history\n";
        cout << "7) Logout\n";
        cout << "--------------------------------\n";
        cin >> userChoice;

        while (userChoice > 7 || userChoice < 1) {
            cout << "Please enter valid number choice: ";
            cin >> userChoice;
        }
        
        switch (userChoice) {
        case 1: searchForProducts();
            break;
        case 2: //viewCart();
            break;
        case 3: //checkPointsBalance();
            break;
        case 4: updateInformation();
            break;
        case 5: changePassword();
            break;
        case 6: checkOrderHistory();
            break;
        case 7:
            return;
        case 8: return;
        default:
            break;
        }

    }

}

//update the customer name, email and phone number
void Customer::updateInformation() {

    string input;
    string firstName, lastName, emailAddress;
    long long phoneNumber;
    bool enteredCorrectly = false;


    cout << "Please enter your first name: ";
    cin >> input;

    firstName = input;

    cout << "Please enter your last name: ";
    cin >> input;

    lastName = input;

    cout << "Please enter your phone number: ";
    cin >> input;

    while (!isValidPhoneNumber(input)) {
        cout << "Please enter a valid phone number, digits only: ";
        cin >> input;
    }

    phoneNumber = stoll(input);

    cout << "Please enter your email address: ";
    cin >> input;

    

    while (!isValidEmail(input)) {
        cout << "Please enter a valid email address: ";
        cin >> input;
    }

    emailAddress = input;

    queue<string> result = query(1, "select customerID from customer where customerID = " +
        to_string(this->userID));

    if (!result.empty()) {
        query(0, "update customer set firstName = '" + firstName + "', lastName = '"
            + lastName + "', emailAddress = '" + emailAddress + "', phoneNumber = " +
            to_string(phoneNumber) + " where customerID = " + to_string(userID));
    }
    
    else {
        query(0, "insert into customer values (" + to_string(userID) + ", '" + firstName
            + "', '" + lastName + "', '" + emailAddress + "', " + to_string(phoneNumber) + ")");
    }
    

    this->userFirstName = firstName;
}

//will check for orders that they have and let them also choose which ones to view and view the basket 
// (viewing the basket will be a seperate function so the cart function can call it too)
void Customer::checkOrderHistory() {
    
    //select orderNumber, totalCost from orderHistory where userID = this->userID;
}

//changes the password just because the user is logged in, no verification
void Customer::changePassword() {

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

//this will eventually search for products and list them
void Customer::searchForProducts() {

    string input;
    bool searching = true;

    while (searching) {
        cout << "Please enter the name of the product you're looking for. Type 'Done' to stop searching.\n";
        cin >> input;

        if (input.compare("Done") == 0 || input.compare("done") == 0)
            return;
    }
}

