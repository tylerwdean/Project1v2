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
        updateInformation();
        this->userFirstName = query(1, "select firstName from customer where customerID = " + to_string(userID)).front();
    }

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
        case 5: changePasswordLoggedIn();
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
    string firstName, lastName, emailAddress, phoneNumber;
    bool enteredCorrectly = false;


    cout << "Please enter your first name: ";
    firstName = getLine();

    cout << "Please enter your last name: ";
    lastName = getLine();

    cout << "Please enter your phone number: ";
    phoneNumber = getPhoneNumber();

    cout << "Please enter your email address: ";
    emailAddress = getEmail();

    while (isTakenEmail(emailAddress)) {

        if (stoi(query(1, "select customerID from customer where emailAddress = '" + changeApostraphe(emailAddress) + "'").front()) == userID)
            break;

        cout << "That email address is taken, please enter an available one: ";
        emailAddress = getEmail();
    }

    queue<string> result = query(1, "select customerID from customer where customerID = " +
        to_string(this->userID));

    if (!result.empty()) {
        query(0, "update customer set firstName = '" + changeApostraphe(firstName) + "', lastName = '"
            + changeApostraphe(lastName) + "', emailAddress = '" + changeApostraphe(emailAddress) + "', phoneNumber = " +
            phoneNumber + " where customerID = " + to_string(userID));
    }
    
    else {
        query(0, "insert into customer values (" + to_string(userID) + ", '" + changeApostraphe(firstName)
            + "', '" + changeApostraphe(lastName) + "', '" + changeApostraphe(emailAddress) + "', " + phoneNumber + ")");
    }

    this->userFirstName = firstName;
}

//will check for orders that they have and let them also choose which ones to view and view the basket 
// (viewing the basket will be a seperate function so the cart function can call it too)
void Customer::checkOrderHistory() {
    
    //select orderNumber, totalCost from orderHistory where userID = this->userID;
}

//this will eventually search for products and list them
void Customer::searchForProducts() {

    string search;
    string product;
    bool searching = true;
    queue<string> result;
    int i, interestedProduct;

    cout << "\nPlease enter the name of the product you're looking for. Type 'Done' to stop searching.\n";
    search = getLine();

    while (searching) {

        if (search.compare("Done") == 0 || search.compare("done") == 0)
            return;

        result = query(1, "Select productName from inventory where productName like '%" + changeApostraphe(search) + "%'");

        if (result.empty()) {
            cout << "No results\n";
            continue;
        }

        for(i = 1; !result.empty(); i++) {

            product = result.front();
            result.pop();
            cout << i << ") " + product + "\n";
            cout << "------------------------------\n";
        }
        cout << "Enter the number for the product you want to look at more,the name of an item you want to search for, or 'done' to stop searching.\n";
        search = getLine();

        try {
            interestedProduct = stoi(search);
            if (interestedProduct > i || interestedProduct < 1) {
                cout << "Number not in acceptable range. Please enter a product name or 'done' to stop searching\n";
                search = getLine();
            }
        }
        catch (...) {
            continue;
        }

        result = query(3, "select productName, productPrice, quantityStocked, productID from inventory where productName = '" + changeApostraphe(product) + "'");

        cout << "Name: " + result.front();
        result.pop();
        cout << "\nPrice: " + result.front();
        result.pop();
        cout << "\nQuantity Stocked: " + result.front();
        cout << "\nEnter quantity wanted or -1 to search again.\n";
        search = getNumberInRange(-1, stoi(result.front()));

        if (stoi(search) == -1) {
            cout << "\nPlease enter the name of the product you're looking for. Type 'Done' to stop searching.\n";
            search = getLine();
        }
        
        result.pop();
        query(0, "insert into cart values (");
    }
}

