#pragma once
#include <string>

enum Role {customer, employee, manager};

class User{

public:
    void mainMenu();
    void loggedIn();
    void changePassword();

protected:
    int userID = -1;
    std::string userFirstName;
    Role userRole;

    void verifyUser();
    void createUser(Role userRole);
};