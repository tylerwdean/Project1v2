#include "Manager.h"
#include "Util.h"
#include <string>
#include <iostream>

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
	
	int userChoice = 0;

	while (userChoice != 9) {
		cout << "Hello " + this->userFirstName + "!\n";
		cout << "What would you like to do?\n";
		cout << "1) Add new Employee\n";
		cout << "2) Promote/Add Manager\n";
		cout << "3) Add new Product\n";
		cout << "4) View all Employees\n";
		cout << "5) Change Employee's Rate\n";
		cout << "6) Fire Employee\n";
		cout << "7) See Business Revenue\n";
		cout << "8) Change password\n";
		cout << "9) Log out\n";
		cout << "---------------------------------\n";
		cin >> userChoice;

		while (userChoice > 9 || userChoice < 1) {
			cout << "Please enter valid choice\n";
			cin >> userChoice;
		}

		switch (userChoice) {
		case 1: addEmployee(Role::employee);
				break;
		case 2: addManager();
			break;
		case 3: createItem();
			break;
		case 4: viewAllEmployees();
			break;
		case 5: updateEmployeeRate();
			break;
		case 6:fireEmployee();
			break;
		case 7:viewBusinessRevenue();
			break;
		case 8: changePasswordLoggedIn();
			break;
		case 9: return;
		default:
			break;
		}
	}

	return;
}

void Manager::addEmployee(Role newRole) {

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

	string query0 = "insert into login values('" + hashedPassword + "', '" + salter + "', '" + newUsername + "')";
	query(0, query0);

	string employeeUserID = query(1, "select userID from login where username = '" + newUsername + "'").front();
	query(0, "insert into employee values(" + employeeUserID + ", " + to_string(hourlyRate) + ", " + to_string(employeeSS) + ", "
		+ to_string(employeePhoneNumber) + ", '" + employeeFirstName + "', '" + employeeLastName + "')");

	string role;

	switch (newRole) {
	case 1: role = "employee";
		break;
	case 2: role = "manager";
		break;
	default: role = "customer";
	}

	query(0, "insert into roles values (" + employeeUserID + ", '" + role + "')");

	cout << "Employee added successfully\n";
	cout << "-------------------------------------------\n";

	return;
}

void Manager::fireEmployee() {

	string input;
	string employeeID;

	cout << "Please enter the Employee ID of the person you want to remove\n";
	cin >> input;

	if (input.compare("quit") == 0) {
		return;
	}

	while (!isValidEmployeeID(input)) {
		cout << "That is not a valid ID. Please enter a different one or type 'quit' to quit\n";
		cin >> input;

		if (input.compare("quit") == 0) {
			return;
		}
	}

	employeeID = input;

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

	cout << "Please enter the Employee ID of the employee you would like to change the rate of. Enter 'quit' to quit.\n";
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
	double totalRevenue = stod(query(1, "select sum(orderTotal) from orderHistory").front());
	double rate;
	int hours;
	queue<string> result = query(2, "select rateWorked, hoursWorked from hoursWorked");


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
	cin >> input;

	while (stoi(input) > 2 || stoi(input) < 1) {
		cout << "Please enter valid choice: \n";
	}

	userChoice = input;

	if (stoi(input) == 1) {
		addEmployee(Role::manager);
	}

	else {
		cout << "Please enter the employee ID of who you want to promote: ";
		cin >> input;

		while (!isValidEmployeeID(input)) {
			cout << "Please enter a valid employee ID: ";
			cin >> input;
		}

		employeeID = input;

		query(0, "update roles set role = 'manager' where userID = '" + employeeID + "'");
	}
}