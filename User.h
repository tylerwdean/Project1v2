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

    int userID;
    std::string userFirstName;

    /*  This function logs the customer in after they enter their information
    * 
    *   Doesn't return but instead redirects to new menus
    */
    void logIn();

    /* This function gets the username and password
    * 
    *   Returns true if the customer entered correct information
    */
    bool verifyUser();

    /*  This funciton changes the password using the customer's email and phone number but
    *   can only change it if the customer entered that information 
    * 
    *   Doesn't return anything but will update the password if they can verify identity
    */
    void changePassword();

    /*  This function is used to register a new user and is passed the appropriate role
    * 
    *   Doesn't return anything but adds new login information to login table
    */
    void createUser(Role userRole);
};