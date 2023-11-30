#include "Manager.h"
#include "Util.h"
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

Manager::Manager(int userID) : Employee(){
	this->userID = userID;
	this->userRole = manager;

	string query0 = "select firstName from employee where employeeID = " + to_string(userID);
	queue<string> result = query(1, query0);

	if (!result.empty())
		this->userFirstName = result.front();

	else {
		this->userFirstName = query(1, "select userName from login where userID = " + to_string(userID)).front();
	}
}



void Manager::mainMenu() {
	
	string userChoice = "0";

	while (stoi(userChoice) != 12) {
		cout << "\nHello " + this->userFirstName + "!\n";
		cout << "What would you like to do?\n";
		cout << "1) Add new Employee\n";
		cout << "2) Promote/Add Manager\n";
		cout << "3) Add hours worked\n";
		cout << "4) Add new Product\n";
		cout << "5) Generate Discount Code\n";
		cout << "6) View all Employees\n";
		cout << "7) Change Employee's Rate\n";
		cout << "8) Fire Employee\n";
		cout << "9) See Business Revenue\n";
		cout << "10) Change password\n";
		cout << "11) Update Information\n";
		cout << "12) Log out\n";
		cout << "---------------------------------\n";
		userChoice = getNumberInRange(1, 12);

		switch (stoi(userChoice)) {
		case 1: addEmployee(Role::employee);
				break;
		case 2: addManager();
			break;
		case 3: addHoursWorked();
			break;
		case 4: createItem();
			break;
		case 5: generateDiscountCode();
			break;
		case 6: viewAllEmployees();
			break;
		case 7: updateEmployeeRate();
			break;
		case 8:fireEmployee();
			break;
		case 9:viewBusinessRevenue();
			break;
		case 10: changePasswordLoggedIn();
			break;
		case 11: updateInformation();
			break;
		case 12: return;
		default:
			break;
		}
	}

	return;
}

void Manager::generateDiscountCode() {
	
	string discountPercentage;
	
	cout << "How much will the discount code be off? (Digits only, 0-100): ";
	discountPercentage = getNumberInRange(0, 100);

	Employee::generateDiscountCode(discountPercentage);
}

void Manager::addEmployee(Role newRole) {

	string employeeFirstName, employeeLastName, newUsername, newPassword, employeeSS, employeePhoneNumber, hourlyRate;

	cout << "----------------------------\n";
	cout << "Please enter new employee's first name or 'quit' to go back\n";
	employeeFirstName = getLine();
	
	if (employeeFirstName.compare("quit") == 0) {
		return;
	}

	cout << "Please enter new employee's last name\n";
	employeeLastName = getLine();

	cout << "Please enter their hourly rate\n";
	hourlyRate = getDoubleInRange(0.0, 100.0);

	cout << "Please enter new employee's Social Security Number\n";
	employeeSS = getNumberInRange(100000000, 999999999);

	cout << "Please enter the employee's phone number, digits only\n";
	employeePhoneNumber = getNumberInRange(1000000000, 9999999999);


	cout << "Please enter the new Employee's Username\n";
	newUsername = getLine();

	while (isTakenUsername(newUsername)) {
		cout << "That username is taken, please enter another one\n";
		newUsername = getLine();
	}


	cout << "Please enter the desired password\n";
	newPassword = getLine();

	while (!isValidPassword(newPassword)) {
		cout << "Please enter a password with 8 characters, an uppercase letter, a lowercase letter and a number\n";
		newPassword = getLine();
	}

	string salter = generateRandomString(16);
	string hashedPassword = hashString(newPassword, salter);

	string query0 = "insert into login values('" + hashedPassword + "', '" + salter + "', '" + changeApostraphe(newUsername) + "')";
	query(0, query0);

	string employeeUserID = query(1, "select userID from login where username = '" + newUsername + "'").front();
	query(0, "insert into employee values(" + employeeUserID + ", " + hourlyRate + ", " + employeeSS + ", "
		+ employeePhoneNumber + ", '" + changeApostraphe(employeeFirstName) + "', '" + changeApostraphe(employeeLastName) + "')");

	string role;

	switch (newRole) {
	case 1: role = "employee";
		break;
	case 2: role = "manager";
		break;
	default: role = "customer";
	}

	query(0, "insert into roles values (" + employeeUserID + ", '" + role + "')");

	query(0, "insert into hiringManager values (" + employeeUserID + ", " + to_string(userID) + ")");

	cout << "Employee added successfully\n";
	cout << "-------------------------------------------\n";

	return;
}

void Manager::fireEmployee() {

	string input;
	string employeeID;

	cout << "Please enter the Employee ID of the person you want to remove or -1 to quit\n";
	employeeID = getNumberInRange(-1, 100000000);

	while (!isValidEmployeeID(employeeID)) {
		
		if (input.compare("-1") == 0) {
			return;
		}
		
		cout << "That is not a valid ID. Please enter a different one.\n";
		employeeID = getNumberInRange(-1, 100000000);
	}

	queue<string> result = query(2, "select firstName, lastName from employee where employeeID = " + employeeID);
	cout << "Are you sure you want to fire " + result.front() + " ";
	result.pop();
	cout << result.front() << "? y/n: ";
	cin >> input;

	if (input.compare("y") == 0) {
		string query0 = "delete from login where userID = " + employeeID;
		query(0, query0);
		cout << "Employee is deleted\n";
		cout << "--------------------------------\n";
	}

	else {
		cout << "Did not delete employee from database\n";
		cout << "------------------------------------------\n";
	}

	return;
}

void Manager::createItem() {

	string input;
	string productName;
	string productPrice;
	string startingQuantity;
	bool enteredCorrectly = false;

	cout << "Please enter the name of the product you want to add. Type 'quit' to quit. Type '_' instead of spaces.\n";
	productName = getLine();
	std::replace(productName.begin(), productName.end(), '_', ' '); 

	while (!enteredCorrectly) {

		if (productName.compare("quit") == 0)
			return;

		cout << "You entered: '" + productName + "'. Is that what you want to keep it as? y/n\n";
		cin >> input;

		if (input.compare("y") == 0)
			enteredCorrectly = true;

		else {
			cout << "Please enter the name of the product you want to add.\n";
			productName = getLine();
			std::replace(productName.begin(), productName.end(), '_', ' ');
		}
	}

	enteredCorrectly = false;

	cout << "What is the price of the item?\n";
	productPrice = getDoubleInRange(0.01, 10000.0);

	cout << "How many of these items do you want to stock initially?\n";
	startingQuantity = getNumberInRange(0, 10000);

	string query0 = "insert into inventory values('" + changeApostraphe(productName) + "', " + productPrice +
		", " + startingQuantity + ")";
	query(0, query0);
	cout << "Product successfully added\n";
	cout << "------------------------------\n\n";

	return;
}

void Manager::updateEmployeeRate() {

	string newRate;
	string input;
	string employeeID;
	bool enteredCorrectly = false;

	cout << "Please enter the Employee ID of the employee you would like to change the rate of. Enter 'quit' to quit.\n";
	employeeID = getNumberInRange(0, 10000);

	while (!enteredCorrectly) {

		if (isValidID(std::stoi(input))) {
			enteredCorrectly = true;
		}

		else {
			cout << "Please enter valid employee ID\n";
			employeeID = getNumberInRange(0, 10000);
		}
	}

	enteredCorrectly = false;

	if (employeeID.compare(to_string(userID)) == 0) {
		cout << "You can't change how much you're paid!!\n";
		return;
	}

	cout << "Please enter the new rate of the employee.\n";
	newRate = getDoubleInRange(0.0, 100.0);

	string query0 = "update employee set hourlyRate = " + newRate + " where userID = " + to_string(userID);
	cout << "Rate has been updated to " + newRate + ".\n";
	cout << "------------------------------------\n";

	return;
}

void Manager::viewAllEmployees() {
	queue<string> result = query(3, "select employeeID, firstName, lastName from employee");
	string userID, firstName, lastName;

	printf("---------------|---------------|-----\n");

	while (!result.empty()) {
		userID = result.front();
		result.pop();
		firstName = result.front();
		result.pop();
		lastName = result.front();
		result.pop();
		printf("%-15s|%-15s|%-5s\n", firstName.c_str(), lastName.c_str(), userID.c_str());

	}

	cout << endl;

	return;
}

void Manager::viewBusinessRevenue() {
	
	double totalCost = 0.0;
	queue<string> result = query(1, "select totalCost from orders");
	double totalRevenue = 0.0;

	while (!result.empty()) {
		totalRevenue += stod(result.front());
		result.pop();
	}

	double rate;
	int hours;

	result = query(2, "select rateWorked, hoursWorked from hoursWorked");

	while (!result.empty()) {
		rate = stod(result.front());
		result.pop();
		hours = stoi(result.front());
		result.pop();
		totalCost += (rate * hours);
	}
	double netProfit = totalRevenue - totalCost;


	printf("Total Revenue: %.2f\n", totalRevenue);
	printf("Total Cost: %.2f\n", totalCost);
	printf("Total Net Profit: %.2f\n", netProfit);

	return;
}

void Manager::addManager() {

	string input;
	string userChoice, employeeID;

	cout << "Would you like to:\n";
	cout << "1) Add new manager\n";
	cout << "2) Promote employee\n";
	userChoice = getNumberInRange(1, 2);

	if (stoi(userChoice) == 1) {
		addEmployee(Role::manager);
	}

	else {
		cout << "Please enter the employee ID of who you want to promote or 0 to quit: ";
		employeeID = getNumberInRange(0, 10000);

		if (employeeID.compare("0") == 0)
			return;

		while (!isValidEmployeeID(employeeID)) {
			cout << "Please enter a valid employee ID: ";
			employeeID = getNumberInRange(0, 10000);
		}

		query(0, "update roles set role = 'manager' where userID = '" + changeApostraphe(employeeID) + "'");
	}
}