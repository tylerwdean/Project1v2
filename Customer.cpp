#include <string>
#include "Util.h"
#include "Customer.h"
#include "StringNode.h"

//initializes the user and updates the first name for menu purposes
Customer::Customer(int userID) {
    this->userID = userID;
    queue<string> firstName = query(1, "select firstName from customer where customerID = " + to_string(userID));
    
    if (!firstName.empty()) {
        this->userFirstName = firstName.front();
    }

    else {
        //if first login has no information, will add them
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
        case 2: viewCart();
            break;
        case 3: checkPointsBalance();
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

//prints the current balance of points
void Customer::checkPointsBalance() {

    queue<string> result = query(1, "select points from customer where customerID = " + to_string(userID));

    cout << "Your current point balance is " + result.front() << " points.\n";

    return;
}

//shows the customer their current cart
void Customer::viewCart() {

    queue<string> result;
    result = query(2, "select productID, quantity from cart where customerID = " + to_string(this->userID));
    string input;
    double subTotal = 0.0;
    string itemName, itemCost, quantity;

    cout << "\n-------------------------------------\n";
    printf("%-20s|%7s|%s", "Product Name", "Price", "Quantity");
    cout << "\n-------------------------------------\n";

    for (int i = 1; !result.empty(); i++) {

        queue<string> item = query(2, "select productName, price from inventory where productID = " + result.front());
        result.pop();

        itemName = item.front();
        item.pop();

        itemCost = item.front();
        item.pop();

        quantity = result.front();
        result.pop();

        subTotal += (stod(itemCost) * stoi(quantity));

        printf("%-20s|%7s|%s\n", itemName.c_str(), itemCost.c_str(), quantity.c_str());
    }

    cout << "--------------------------------------\n";
    cout << "Subtotal: ";
    printf("%.2f\n", subTotal);
    cout << "--------------------------------------\n";
    cout << "Enter 1 to exit, 2 to check out\n";
    input = getNumberInRange(1, 2);

    if (stoi(input) == 2) {
        checkOut();
    }
}

//turns the customer's cart into an order, probably could be functionalized but whatever
void Customer::checkOut() {

    queue<string> result = query(2, "select productID, quantity from cart where customerID = " + to_string(this->userID));
    double totalCost = 0.0;
    string productID, quantity;
    string discountCode;

    query(0, "insert into orderHistory values(" + to_string(userID) + ", NULL, 'unfinished')");
    string orderID = query(1, "select orderID from orderHistory where customerID = " + to_string(userID)
        + " and discountCode = 'unfinished'").front();

    //adds each item from cart into the order and removes it from the cart
    while (!result.empty()) {

        productID = result.front();
        result.pop();
        quantity = result.front();
        result.pop();

        queue<string> price = query(2, "select price from inventory where productID = " + productID);
        query(0, "insert into orders values ('" + orderID + "', " + productID + ", " + quantity + ")");
        totalCost += stod(price.front()) * stoi(quantity);

        query(0, "delete from cart where productID = " + productID + " and customerID = " + to_string(userID));
    }

    discountCode = getDiscountCode();

    //gets their points and asks how many they want to use, updates their points based on that too
    string points = query(1, "select points from customer where customerID = " + to_string(userID)).front();
    cout << "You have " + points + " points. How many do you want to use?\n";

    //calculates the max amount of points able to be used and sets input range from that
    int maxPoints = (int)(totalCost * 10);

    string pointsUsed = "";

    if (stoi(points) > maxPoints) {
        pointsUsed = getNumberInRange(0, maxPoints);
    }

    else {
        pointsUsed = getNumberInRange(0, stoi(points));
    }

    query(0, "update customer set points -= " + pointsUsed + " where customerID = " + to_string(userID));

    //reducts the overall cost by the points
    totalCost = totalCost - stod(pointsUsed) / 10;

    //calculates and inserts the final price
    if (discountCode.compare("none") == 0) {
        query(0, "update orderHistory set totalCost = " + to_string(totalCost) + ", discountCode = NULL" + " where orderID = " + orderID);
    }

    else {

        totalCost = (100-stod(query(1, "select percentageOff from discount where discountCode = '" + changeApostraphe(discountCode) + "'").front()))/100 * totalCost;

        query(0, "update orderHistory set totalCost = " + to_string(totalCost) + ", discountCode = '" + changeApostraphe(discountCode) + "' where orderID = " + orderID);
    }
    
    //customer is done and checked out
    cout << "You're checked out! Your total came out to: ";
    printf("%.2f\n", totalCost);
    cout << "Your order ID = " << orderID << "\n";

    int addedPoints = (int)totalCost;

    //transaction is done, adds a point per dollar spent
    query(0, "update customer set points += " + to_string(addedPoints) + " where customerID = " + to_string(userID));

    return;
}

//update the customer name, email and phone number
void Customer::updateInformation() {

    string input;
    string firstName, lastName, emailAddress, phoneNumber;
    bool enteredCorrectly = false;

    //gets customer information
    cout << "Please enter your first name: ";
    firstName = getLine();

    cout << "Please enter your last name: ";
    lastName = getLine();

    cout << "Please enter your phone number: ";
    phoneNumber = getPhoneNumber();

    cout << "Please enter your email address: ";
    emailAddress = getEmail();

    while (isTakenEmail(emailAddress)) {

        //will only let them put in their previous email
        if (stoi(query(1, "select customerID from customer where emailAddress = '" + changeApostraphe(emailAddress) + "'").front()) == userID)
            break;

        cout << "That email address is taken, please enter an available one: ";
        emailAddress = getEmail();
    }

    queue<string> result = query(1, "select customerID from customer where customerID = " +
        to_string(this->userID));

    //if the customer isn't added to the database, it adds new information
    if (!result.empty()) {
        query(0, "update customer set firstName = '" + changeApostraphe(firstName) + "', lastName = '"
            + changeApostraphe(lastName) + "', emailAddress = '" + changeApostraphe(emailAddress) + "', phoneNumber = " +
            phoneNumber + " where customerID = " + to_string(userID));
    }
    //if customer is added, it updates the info
    else {
        query(0, "insert into customer values (" + to_string(userID) + ", '" + changeApostraphe(firstName)
            + "', '" + changeApostraphe(lastName) + "', '" + changeApostraphe(emailAddress) + "', " + phoneNumber + ")");
    }

    this->userFirstName = firstName;
}

//will check for orders that they have and let them also choose which ones to view and displays the order
void Customer::checkOrderHistory() {
    
    StringNode *head, *cursor;
    int i = 0;

    queue<string> result = query(2, "select orderID, totalCost from orderHistory where customerID = " + to_string(userID));

    if (result.empty()) {
        cout << "You don't have any orders\n";
        return;
    }

    head = new StringNode();
    cursor = head;

    for (i = 1; !result.empty(); i++) {

        cursor->data = result.front();
        cout << i <<") Order Number: " << result.front();
        result.pop();
        cout << " Total: " << result.front() << "\n";
        cursor->next = new StringNode();
        cursor = cursor->next;
        result.pop();
    }

    cout << "What order would you like to look at? Enter -1 to quit\n";
    string input = getNumberInRange(-1, i);

    if (stoi(input) < 1)
        return;

    string orderID;
    cursor = head;
    //loops through the linked list of orders so they can choose the one they want
    for (int i = 1; i < stoi(input); i++) {
        cursor = cursor->next;
    }

    displayOrder(cursor->data);

    return;
}

//given orderID, displays the items in the order 
void Customer::displayOrder(string orderID) {

    queue<string> order = query(2, "select productID, quantity from orders where orderID = " + orderID);
    queue<string> result;
    string query0;

    cout << "Order #" + orderID << "\n";
    cout << "-------------------------------\n";
    while (!order.empty()) {
        query0 = "select productName from inventory where productID = " + order.front();
        order.pop();
        result = query(1, query0);
        cout << result.front() << ", " << order.front() << "\n";
        order.pop();
    }

    cout << "-------------------------------\n";
    cout << "Total Cost: " + query(1, "select totalCost from orderHistory where orderID = " + orderID).front() << "\n";
}

//the general search alogrithm
void Customer::searchForProducts() {

    string search;
    string result;
    bool searching = true;

    result = searchProducts();

    if (result.compare("done") == 0)
        return;

    displayItem(result);

    int stock = stoi(query(1, "select quantityStocked from inventory where productID = " + result).front());

    cout << "\nEnter quantity wanted or -1 to search again.\n";
    search = getNumberInRange(-1, stock);
        
    if (search.compare("-1") == 0 || search.compare("0") == 0)
        return;

    addToCart(result, stoi(search));

    cout << "Successfully added to cart!\n";
}

//displays the particular item
void Customer::displayItem(string productID) {
    
    queue<string> result = query(4, "select productName, price, quantityStocked, productID from inventory where productID = " + productID);
    
    cout << "\nName: " + result.front();
    result.pop();
    cout << "\nPrice: " + result.front();
    result.pop();
    cout << "\nQuantity Stocked: " + result.front() << "\n";
}

void Customer::addToCart(string productID, int quantity) {

    if (quantity < 0)
        return;

    if (!query(1, "select productID from cart where customerID = " + to_string(userID) + " and productID = " + productID).empty()) {
        
        query(0, "update cart set quantity += " + to_string(quantity) + " where customerID = " +
            to_string(userID) + " and productID = " + productID);
        query(0, "update inventory set quantityStocked -= " + to_string(quantity) + " where productID = " + productID);
        return;
    }

    query(0, "insert into cart values (" + to_string(userID) + ", " + productID + ", " + to_string(quantity) + ")");
    query(0, "update inventory set quantityStocked -= " + to_string(quantity) + " where productID = " + productID);
}