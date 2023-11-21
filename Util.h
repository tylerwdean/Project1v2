#pragma once
#include <string>
#include <iostream>  
#include <queue>

using namespace std;

/* This function takes an integer representing the number of attributes in the resulting query
* as well as the query iteself in a string
* 
* Returns a queue of strings with each item in the table, length is a multiple of the number of attributes
*/
queue<string> query(int numOfItems, string queryInput);

/*	This function takes two strings and uses them together to create a hash
*
*	Returns a unique hash for the two strings
*/
string hashString(string hash1, string hash2);

/*	This function generates a random string with uppercase letters, takes length as an integer
*
*	Returns the generated string
*/
string generateRandomString(int length);

/*	This function sees if the string contains any characters outside of the accepted ones
* 
*	Returns true if the string doesn't contain unexpected characters
*/
bool isClean(string dirtyString);


/*	This function checks to see if the string contains all the necessary components of a valid password.
*	The password requirements are 8 characters, an uppercase letter, lowercase letter and a number. Additional characters allowed.
*	
*	Returns true if the string meets these requirements.
*/
bool isValidPassword(string password);


/*	This checks to see if the entered username exists.
* 
*	Returns true if there is already a username matching the string
*/
bool isTakenUsername(string username);

/*	This functions checks if the integer ID exists as a user but doesn't check for the particular role of the user
* 
*	Returns true if the userID is in the database
*/
bool isValidID(int userID);

/*	This function checks if the entered number is a valid product code
*	
*	Returns true if product code is linked to a product
*/
bool isValidProductCode(string productCode);

/*	This function checks if the given order is a valid order or not
* 
*	Returns true if it is a valid orderID
*/
bool isValidOrder(int orderID);

/*	This function checks the string to see if it is an email address

	Returns true if it is a valid email address
*/
bool isValidEmail(string emailAddress);

/* This function checks if the string is a number 10 digits long
* 
*	Returns true if it could be a valid phone number
*/
bool isValidPhoneNumber(string input);

/*	This function sees if the email is taken or not
* 
*	Returns true if you can't use the email
*/
bool isTakenEmail(string email);

/*	This function checks if the discount code is taken
* 
*	Returns true if taken
*/
bool isTakenDiscountCode(string discountCode);

/*  Returns if the string given is a valid employee id
*/
bool isValidEmployeeID(std::string userID);


/*	Reads the latest input and if it's not a valid input, displays message to screen asking for it again. Range is inclusive both ends. 
* 
*	Returns the number the user enters within the set range as a string
*/
string getNumberInRange(long long low, long long high);

/*	Reads the latest input and if it's not a valid input, displays message to screen asking for it again
*
*	Returns the email that the user enteres
*/
string getEmail();

/*	Reads the current line the user enters and ensures that it is clean
* 
*	Returns the string entered by the user
*/
string getLine();

/*	Reads the latest input and sees if its a good double in the range 
* 
*	Returns the string with the number
*/
string getDoubleInRange(double low, double high);

string getPhoneNumber();

string changeApostraphe(string input);

string getDiscountCode();

bool isValidDiscountCode(string discountCode);

string getLineInvisible();