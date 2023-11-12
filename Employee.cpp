#include "Employee.h"
#include <iostream>
#include <string>

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
	case 5: viewEarnings();
		break;
	case 6: changePassword();
		break;
	case 7: updateInformation();
		break;
	case 8: return;
	default:
		break;
	}
}
