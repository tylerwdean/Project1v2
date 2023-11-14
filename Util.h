#pragma once
#include <string>
#include <iostream>  
#include <queue>

using namespace std;

//this function takes a string and return the resulting item, only good for single item queries
queue<string> query(int numOfItems, string queryInput);

//this function sanitizes a string that the user inputs to ensure it doesn't interfere with any SQL queries
bool isClean(string dirtyString);

//this function takes a given two inputs and creates a unique hash that combines the two of them
string hashString(string hash1, string hash2);

//this function checks password requirements
//same as Apple, 8 char, number uppercase and lowercase, special characters _!*-$?%&
bool isValid(string password);

//this function generates a random string for salting
string generateRandomString(int length);

//this function checks to ensure the username is valid
bool isTaken(string username);

bool isValidID(int userID);

bool isValidProductCode(int productCode);

bool isValidOrder(int orderID);