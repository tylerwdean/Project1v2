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
}

void Manager::addEmployee() {

	string input;
	string employeeFirstName, employeeLastName, newUsername, newPassword;
	int employeeSS, employeePhoneNumber, hourlyRate;


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

	hourlyRate = input;

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

	cout << "Please enter the employee's phone number, digits only";
	cin >> input;

	if (input.compare("quit") == 0) {
		return;
	}

	employeePhoneNumber = std::stoi(input);

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

	while (isTaken(input)) {
		cout << "That username is taken, please enter another one\n";
		cin >> input;
	}

	newUsername = input;

	cout << "Please enter the desired password\n";
	cin >> input;

	while (!isValid(input)) {
		cout << "Please enter a password with 8 characters, an uppercase letter, a lowercase letter and a number\n";
		cin >> input;
	}

	newPassword = input;

	string salter = generateRandomString(16);
	string hashedPassword = hashString(newPassword, salter);

	//insert into login values (newUsername, salter, hashedPassword)
	cout << "Employee added to database successfully\n";
	//int employeeUserID = (select userID from login where username = newusername)
	//insert into employee values (employeeUserID, hourlyRate, 
}