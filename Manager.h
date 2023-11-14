#pragma once
#include <string>
#include "Employee.h"

using namespace std;

class Manager : public Employee {
public:

    //Manager(int userID);

    void mainMenu();
    //hires employee
    void addEmployee();
    //fires
    void fireEmployee();
    //lists all employees with name and ID
    void viewAllEmployees();

    void updateEmployeeRate();

    void createItem();

    void viewBusinessRevenue();
};