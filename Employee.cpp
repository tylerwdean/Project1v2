#include "Employee.h"
#include <iostream>
#include <string>
#include "Util.h"

using namespace std;

Employee::Employee(int userID) {
	this->userID = userID;
	this->userRole = employee;

	string query0 = "select firstName from employee where employeeID = " + to_string(userID);
	queue<string> result = query(1, query0);

	if (!result.empty())
		this->userFirstName = result.front();

	else {
		this->userFirstName = query(1, "select userName from login where userID = " + to_string(userID)).front();
	}
}

Employee::Employee() {

}

void Employee::mainMenu() {

	string input = "0";

	while (stoi(input) != 9) {

		cout << "Welcome " + this->userFirstName + "!\n";
		cout << "What would you like to do?\n";
		cout << "1) Restock Products\n";
		cout << "2) Delete Customer Order\n";
		cout << "3) Generate Discount Code\n";
		cout << "4) Add hours worked\n";
		cout << "5) See earnings\n";
		cout << "6) Change password\n";
		cout << "7) Update personal information\n";
		cout << "8) View personal information\n";
		cout << "9) Log out\n";
		cout << "------------------------------\n";
		cin >> input;

		while (stoi(input) < 1 || stoi(input) > 9) {
			cout << "Please enter valid choice\n";
			cin >> input;
		}

		switch (stoi(input)) {
		case 1: restockItem();
			break;
		case 2: deleteOrder();
			break;
		case 3: generateDiscountCode("15");
			break;
		case 4: addHoursWorked();
			break;
		case 5: viewEarnings();
			break;
		case 6: changePasswordLoggedIn();
			break;
		case 7: updateInformation();
			break;
		case 8: User::viewInformation();
			break;
		case 9: return;
		default:
			break;
		}
	}
}

void Employee::addHoursWorked() {

	string input;
	string hoursWorked;
	bool enteredCorrectly = false;

	cout << "Please enter the number of hours worked today, rounded to nearest integer.\n";
	hoursWorked = getNumberInRange(0, 9);

	string rate = query(1, "select hourlyRate from employee where employeeID = " + to_string(userID)).front();
	string queryInput = "insert into hoursWorked values ("+ to_string(this->userID) + ", " + rate + ", " + hoursWorked + ")";
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

	cout << "Please enter the amount of inventory being restocked.\n.";
	quantityRestocked = getNumberInRange(0, 10000);

	string query0 = "update inventory set quantityStocked = quantityStocked + " + 
		quantityRestocked + "where productID = " + productCode;
	query(0, query0);

	cout << "Successfully restocked\n";
	cout << "----------------------------------------\n";
	return;
}

void Employee::deleteOrder() {

	bool enteredCorrectly = false;
	string input;
	string orderNumber;

	cout << "Please enter the order number you want to delete or -1 to quit\n";
	orderNumber = getNumberInRange(-1, 10000);

	while (!enteredCorrectly) {
		if (orderNumber.compare("-1") == 0)
			return;
		if (isValidOrder(stoi(orderNumber)))
			enteredCorrectly = true;
		else {
			cout << "Please enter a valid order ID\n";
			orderNumber = getNumberInRange(-1, 10000);
		}
	}

	cout << "Are you sure you want to delete order number " + orderNumber + "? y/n\n";
	cin >> input;

	if (input.compare("y") == 0) {
		string query = "delete from orders where orderID = " + orderNumber;
	}

	return;
}

void Employee::generateDiscountCode(string percentageOff) {
	
	string input;
	string code = generateRandomString(8);

	while (isTakenDiscountCode(code)) {
		code = generateRandomString(8);
	}
	
	cout << "Your discount code is: '" + code + "'\n";
	cout << "This is good for " + percentageOff + "% off.\n";

	string query0 = "insert into discount values (" + to_string(userID) + ", '" + changeApostraphe(code) + "', " + percentageOff + ")";
	query(0, query0);

	cout << "---------------------------------\n";
	return;
}

void Employee::viewEarnings() {

	double moneyEarned = 0.0;
	int hours, daysWorked, totalHours = 0;
	double rate;

	queue<string> result = query(2, "select rateWorked, hoursWorked from hoursWorked where employeeID = " + to_string(userID));

	for (daysWorked = 0; !result.empty(); daysWorked++) {
		rate = stod(result.front());
		result.pop();
		hours = stoi(result.front());
		result.pop();

		moneyEarned += rate * hours;
		totalHours += hours;
	}

	cout << "Total days worked: " + to_string(daysWorked) + "\n";
	cout << "Total hours worked: " + to_string(totalHours) + "\n";
	printf( "Total earnings: %.2f\n", moneyEarned);

}

void Employee::updateInformation() {

	string input;
	string firstName, lastName, phoneNumber, socialSecurity, hourlyRate;

	if (!isValidEmployeeID(to_string(userID))) {

		cout << "Please enter your first name: ";
		firstName = getLine();
		cout << "Please enter your last name: ";
		lastName = getLine();
		cout << "Please enter your SS number: ";
		socialSecurity = getLine();
		cout << "Please enter your hourly rate: ";
		hourlyRate = getDoubleInRange(0.0, 100.0);
		cout << "Please enter your phone number: ";
		phoneNumber = getNumberInRange(1000000000, 9999999999);

		query(0, "insert into employee values(" + to_string(userID) + ", " + hourlyRate + ", " + socialSecurity + ", "
			+ phoneNumber + ", '" + changeApostraphe(firstName) + "', '" + changeApostraphe(lastName) + "')");

		return;

	}

	cout << "Please enter your first name: ";
	firstName = getLine();

	cout << "Please enter your last name: ";
	lastName = getLine();

	cout << "Please enter your phone number: ";
	phoneNumber = getNumberInRange(1000000000, 9999999999);

	string query0 = "update employee set firstName = '" + changeApostraphe(firstName) + "', lastName = '" + changeApostraphe(lastName) + "', phoneNumber = " + phoneNumber + " where employeeID = " + to_string(userID);
	query(0, query0);
}