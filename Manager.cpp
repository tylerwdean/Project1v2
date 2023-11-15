#include "Manager.h"
#include "Util.h"
#include <string>
#include <iostream>

using namespace std;

void Manager::mainMenu() {
	
	int userChoice = 0;

	while (userChoice != 7) {
		cout << "Hello " + this->userFirstName + "!\n";
		cout << "What would you like to do?\n";
		cout << "1) Add new Employee\n";
		cout << "2) Add new Product\n";
		cout << "3) View all Employees\n";
		cout << "4) Change Employee's Rate\n";
		cout << "5) Fire Employee\n";
		cout << "6) See Business Revenue\n";
		cout << "7) Log out\n";
		cout << "---------------------------------\n";
		cin >> userChoice;

		while (userChoice > 7 || userChoice < 1) {
			cout << "Please enter valid choice\n";
			cin >> userChoice;
		}

		switch (userChoice) {
			case 1: addEmployee();
				break;
			case 2: createItem();
				break;
			case 3: viewAllEmployees();
				break;
			case 4: updateEmployeeRate();
				break;
			case 5:fireEmployee();
				break;
			case 6:viewBusinessRevenue();
				break;
			default:
				break;
		}
	}

	return;
}

void Manager::addEmployee() {

	string input;
	string employeeFirstName, employeeLastName, newUsername, newPassword;
	int employeeSS;
	long long employeePhoneNumber;
	double hourlyRate;


	cout << "----------------------------\n";
	cout << "Please enter new employee's first name or 'quit' to go back\n";
	cin >> input;
	
	if (input.compare("quit") == 0) {
		return;
	}

	employeeFirstName = input;

	cout << "Please enter new employee's last name\n";
	cin >> input;

	if (input.compare("quit") == 0) {
		return;
	}

	employeeLastName = input;

	cout << "Please enter their hourly rate\n";
	cin >> input;

	while (std::stod(input) <= 0) {
		cout << "Please enter valid rate";
		cin >> input;
	}

	hourlyRate = std::stod(input);

	cout << "Please enter new employee's Social Security Number\n";
	cin >> input;

	if (input.compare("quit") == 0) {
		return;
	}

	employeeSS = std::stoi(input);

	while (employeeSS < 100000000 || employeeSS > 999999999) {
		cout << "Please enter valid Social Security Number without any extra characters\n";
		cin >> input;
		
		if (input.compare("quit") == 0) {
			return;
		}

		employeeSS = std::stoi(input);
	}

	cout << "Please enter the employee's phone number, digits only\n";
	cin >> input;

	if (input.compare("quit") == 0) {
		return;
	}

	employeePhoneNumber = std::stoll(input);

	while (employeePhoneNumber < 1000000000 || employeePhoneNumber > 9999999999) {
		cout << "Please enter valid phone number\n";
		cin >> input;

		if (input.compare("quit") == 0) {
			return;
		}

		employeePhoneNumber = std::stoi(input);
	}

	cout << "Please enter the new Employee's Username\n";
	cin >> input;

	while (isTakenUsername(input)) {
		cout << "That username is taken, please enter another one\n";
		cin >> input;
	}

	newUsername = input;

	cout << "Please enter the desired password\n";
	cin >> input;

	while (!isValidPassword(input)) {
		cout << "Please enter a password with 8 characters, an uppercase letter, a lowercase letter and a number\n";
		cin >> input;
	}

	newPassword = input;

	string salter = generateRandomString(16);
	string hashedPassword = hashString(newPassword, salter);

	//insert into login values (newUsername, salter, hashedPassword)
	cout << "Employee login added to database successfully\n";
	//int employeeUserID = (select userID from login where username = newusername)
	//insert into employee values (employeeUserID, hourlyRate, 0, 0)
	//insert into employeeContact values (employeeUserID, employeeSS, employeePhoneNumber, employeeFirstName, employeeLastName)
	cout << "Employee information added to ID successfully\n";
	cout << "Employee added successfully\n";
	cout << "-------------------------------------------\n";

	return;
}

void Manager::fireEmployee() {

	string input;

	cout << "Please enter the Employee ID of the person you want to remove\n";
	cin >> input;

	if (input.compare("quit") == 0) {
		return;
	}

	while (!isValidID(std::stoi(input))) {
		cout << "That is not a valid ID. Please enter a different one or type 'quit' to quit\n";
		cin >> input;

		if (input.compare("quit") == 0) {
			return;
		}
	}

	queue<string> result = query(2, "select firstName, lastName from employeeContact where userID = " + input);
	cout << "Are you sure you want to fire " + result.front() + " ";
	result.pop();
	cout << result.front() << "? y/n: ";
	cin >> input;

	if (input.compare("y") == 0) {
		string query0 = "delete from login where userID = " + to_string(userID);
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
	double productPrice;
	int startingQuantity;
	bool enteredCorrectly = false;

	cout << "Please enter the name of the product you want to add. Type 'quit' anytime to quit\n";
	cin >> input;

	while (!enteredCorrectly) {

		if (input.compare("quit") == 0)
			return;

		cout << "You entered: '" + input + "'. Is that what you want to keep it as? y/n\n";
		cin >> input;

		if (input.compare("y") == 0)
			enteredCorrectly = true;

		else if (!isClean(input)) {
			cout << "Not a valid product name, please try again\n";
			cin >> input;
		}

		else {
			cout << "Please enter the name of the product you want to add.\n";
			cin >> input;
		}
	}

	enteredCorrectly = false;

	cout << "What is the price of the item?\n";
	cin >> input;

	while (!enteredCorrectly) {

		if (input.compare("quit") == 0)
			return;

		if (std::stod(input) > 0) {
			enteredCorrectly = true;
			productPrice = std::stod(input);
		}

		else {
			cout << "Please enter a valid price.\n";
			cin >> input;
		}
	}

	enteredCorrectly = false;

	cout << "How many of these items do you want to stock initially?\n";
	cin >> input;

	while (!enteredCorrectly) {

		if (input.compare("quit") == 0)
			return;

		if (std::stoi(input) > -1) {
			enteredCorrectly = true;
			startingQuantity = std::stoi(input);
		}
		
		else {
			cout << "Please enter a valid number.\n";
			cin >> input;
		}
	}

	string query0 = "insert into product values('" + productName + "', '" + to_string(productPrice) + 
		"', " + to_string(startingQuantity) + ")";
	query(0, query0);
	cout << "Product successfully added\n";
	cout << "------------------------------\n\n";

	return;
}

void Manager::updateEmployeeRate() {

	double newRate;
	string input;
	int employeeID;
	bool enteredCorrectly = false;

	cout << "Please enter the Employee ID of the employee you would like to changet the rate of. Enter 'quit' to quit.\n";
	cin >> input;

	while (!enteredCorrectly) {

		if (input.compare("quit") == 0)
			return;

		if (isValidID(std::stoi(input))) {
			enteredCorrectly = true;
			employeeID = std::stoi(input);
		}

		else {
			cout << "Please enter valid employee ID\n";
			cin >> input;
		}
	}

	enteredCorrectly = false;

	cout << "Please enter the new rate of the employee.\n";
	cin >> input;

	while (!enteredCorrectly) {

		if (input.compare("quit") == 0)
			return;

		if (std::stod(input) > 0) {
			enteredCorrectly = true;
			newRate = std::stod(input);
		}
		
		else {
			cout << "Please enter valid rate.\n";
			cin >> input;
		}
	}

	string query0 = "update employee set hourlyRate = " + to_string(newRate) + " where userID = " + to_string(userID);
	cout << "Rate has been updated to " + std::to_string(newRate) + ".\n";
	cout << "------------------------------------\n";

	return;
}

void Manager::viewAllEmployees() {
	//select userID, firstName, lastName from employeeContact
	return;
}

void Manager::viewBusinessRevenue() {
	
	int totalRevenue = 0;
	//totalRevenue = select sum(orderTotal) from orderHistory
	int totalCost = 0;
	//totalCost = sum (earnings) from employeeHours

	int netProfit = totalRevenue - totalCost;

	cout << "Total Revenue: " + std::to_string(totalRevenue) + "\n";
	cout << "Total Cost: " + std::to_string(totalCost) + "\n";
	cout << "Total Net Profit: " + std::to_string(netProfit) + "\n";

	return;
}