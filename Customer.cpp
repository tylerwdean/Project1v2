#include <string>
#include "Util.h"
#include "Customer.h"

Customer::Customer(int userID) {
    this->userID = userID;
    this->userFirstName = query("SELECT firstName FROM customer WHERE ID = " + userID);
}

void Customer::mainMenu() {

    int userChoice = 0;

    while (userChoice != 7) {

        cout << "Hello " + this->userFirstName + "!\n";
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
            cout << "Please enter valid choice: ";
            cin >> userChoice;
        }
        
        switch (userChoice) {
        case 1: searchForProducts();
            break;
        case 2: viewCart();
            break;
        case 3: checkPointsBalance();
            break;
        case 4: updateInformation();
            break;
        case 5: changePassword();
            break;
        case 6: checkOrderHistory();
            break;
        case 7:
            return;
        default:
            break;
        }

    }

}

void Customer::checkOrderHistory() {
    
    //select orderNumber, totalCost from orderHistory where userID = this->userID;
}

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

