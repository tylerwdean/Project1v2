#include "Employee.h"
#include <iostream>
#include <string>
#include "Util.h"

using namespace std;

void Employee::mainMenu() {

	string input;

	cout << "Welcome " + this->userFirstName + "!\n";
	cout << "What would you like to do?\n";
	cout << "1) Restock Products\n";
	cout << "2) Delete Customer Order\n";
	cout << "3) Generate Discount Code\n";
	cout << "4) Add hours worked\n";
	cout << "5) See earnings\n";
	cout << "6) Change password\n";
	cout << "7) Update personal information\n";
	cout << "8) Log out\n";
	cout << "------------------------------\n";
	cin >> input;

	while (stoi(input) < 1 || stoi(input) > 8) {
		cout << "Please enter valid choice\n";
		cin >> input;
	}

	switch (stoi(input)) {
	case 1: restockItem();
		break;
	case 2: deleteOrder();
		break;
	case 3: generateDiscountCode();
		break;
	case 4: addHoursWorked();
		break;
	case 5: //viewEarnings();
		break;
	case 6: //changePassword();
		break;
	case 7: //updateInformation();
		break;
	case 8: return;
	default:
		break;
	}
}

void Employee::addHoursWorked() {

	string input;
	int hoursWorked;
	bool enteredCorrectly = false;

	cout << "Please enter the number of hours worked today, rounded to nearest integer.\n";
	cin >> input;

	while (!enteredCorrectly) {

		if (stoi(input) > 9 || stoi(input) < 0)
			enteredCorrectly = true;

		cout << "Please enter a value in the range of 0-9\n";
		cin >> input;
	}

	hoursWorked = stoi(input);


	string queryInput = "insert into hoursWorked values ("+ to_string(this->userID) + ", " + to_string(hoursWorked) + ")";
	query(0, queryInput);
	
}

void Employee::restockItem() {
	
	string input;
	string productCode, quantityRestocked;
	bool enteredCorrectly = false;

	cout << "Please enter the product code for the item you're restocking. Type 'quit' to quit\n";
	cin >> input;



	while (!enteredCorrectly) {

		if (input.compare("quit") == 0) {
			return;
		}

		if (isValidProductCode(input))
			enteredCorrectly = true;

		else {
			cout << "Please enter a valid product code.\n";
			cin >> input;
		}
	}

	productCode = input;
	enteredCorrectly = false;

	cout << "Please enter the amount of inventory being restocked.\n.";
	cin >> input;

	while (stoi(input) < 0) {
		
		if (input.compare("quit") == 0)
			return;

		if (stoi(input) > -1) 
			enteredCorrectly = true;
		
		else {
			cout << "Please enter a valid value";
			cin >> input;
		}
	}

	quantityRestocked = input;
	enteredCorrectly = false;

	string query0 = "update inventory set quantityStocked = quantityStocked + " + 
		quantityRestocked + "where productID = " + productCode;
	query(0, query0);

	cout << "Successfully restocked\n";
	cout << "----------------------------------------\n";
	return;
}

void Employee::deleteOrder() {

	string input;
	bool enteredCorrectly = false;
	int orderNumber;

	cout << "Please enter the order number you want to delete or type 'quit' to quit\n";
	cin >> input;

	while (!enteredCorrectly) {
		if (input.compare("quit") == 0)
			return;
		if (isValidOrder(stoi(input)))
			enteredCorrectly = true;
		else {
			cout << "Please enter a valid order ID\n";
			cin >> input;
		}
	}

	orderNumber = stoi(input);
	enteredCorrectly = false;

	cout << "Are you sure you want to delete order number " + to_string(orderNumber) + "? y/n\n";
	cin >> input;

	if (input.compare("y") == 0) {
		string query = "delete from orders where orderID = " + to_string(orderNumber);
	}

	return;
}

void Employee::generateDiscountCode() {
	
	string input;
	string code = generateRandomString(8);
	
	cout << "Your discount code is: '" + code + "'";
	cout << "This is good for 15% off.\n";

	string query0 = "insert into discount values (" + to_string(userID) + ", '" + code + "', 15)";
	query(0, query0);

	cout << "---------------------------------\n";
	return;
}

