#pragma once
#include <string>

enum Role {customer, employee, manager};

class User{

public:

    /*  This function hosts the main login menu
    * 
    *   Function ends when user wants to quit the application
    */
    void mainMenu();

    
    

protected:
    int userID = -1;
    std::string userFirstName;
    //Role userRole;
    
    //void changePassword();

    /*  This function starts the login process
    */
    void logIn();
    bool verifyUser();
    void createUser(Role userRole);
};