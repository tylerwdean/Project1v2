#pragma once
#include <string>

enum Role {customer, employee, manager};

class User{

public:
    void mainMenu();
    void loggedIn();

protected:
    int userID = -1;
    std::string userFirstName;

    void verifyUser();
    void createUser(Role userRole);
};