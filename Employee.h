#pragma once
#include <string>
#include "User.h"

using namespace std;

class Employee : public User {
public:
    //in the construction include this line to update name: 
    //userFirstName = query("SELECT firstName FROM customer WHERE ID = " + userID);
    //Employee(int userID);

    void mainMenu();
    void restockItem();
    //void viewEarnings();
    //void addHoursWorked();
    //void updateInformation();
    void deleteOrder();
    void generateDiscountCode();
};