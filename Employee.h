#pragma once
#include <string>
#include "User.h"

using namespace std;

class Employee : public User {
public:

    /*  Basic constructor with the userID
    * 
    *   Sets up basic information such as gathering name and settings role
    */
    Employee(int userID);

    Employee();

    /*  The main menu for the employee
    * 
    *   Ends when they logout
    */
    void mainMenu();

    /*  Will allow employee to add quantity of goods to table provided they enter product code
    * 
    *   Updates the database if entered correctly
    */
    void restockItem();


    /*  Allows employee to add their hours worked for the day, would be nice to have it include the date on the database side
    * 
    *   Updates hoursWorked
    */
    void addHoursWorked();

    /*  Function will update the information but requires different prompts for employees
    * 
    *   Updates employee table
    */
    //void updateInformation();

    /*  Allows employee to delete an order given the order number
    * 
    *   Removes order from orders
    */
    void deleteOrder();

    /*  Generates a 16 digit unique discount code
    * 
    *   Prints code out to screen and also adds it to database
    */
    void generateDiscountCode(string percentageOff);

    /*  Shows the total hours worked and how much has been made
    * 
    *   Prints information to the screen
    */
    void viewEarnings();

    /*  Function updates their name and phone number
    * 
    *   Updates preffered name and phone number
    */
    void updateInformation();
};