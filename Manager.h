#pragma once
#include <string>
#include "Employee.h"

using namespace std;

class Manager : public Employee {
public:

    Manager(int userID);

    void mainMenu();
    //hires employee
    void addEmployee(Role newRole);
    //fires
    void fireEmployee();
    //lists all employees with name and ID
    void viewAllEmployees();
    //updates employee rate given their ID
    void updateEmployeeRate();
    //creates an inventory item
    void createItem();
    //views the costs and profit from orders and employees
    void viewBusinessRevenue();

    void addManager();

    void generateDiscountCode();
};