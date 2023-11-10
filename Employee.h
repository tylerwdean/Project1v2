#pragma once
#include <string>
#include "User.h"

using namespace std;

class Employee : public User {
public:
    //in the construction include this line to update name: 
    //userFirstName = query("SELECT firstName FROM customer WHERE ID = " + userID);
    Employee(int userID);

    void mainMenu();
    void restockItem(int productID, int quantity);
    void viewHoursWorked();
    void addHoursWorked();
    void viewRate();
    void updateInformation();
};