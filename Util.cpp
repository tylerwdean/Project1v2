#include <iostream>
#include <functional>
#include<chrono>   //time for random seed
#include "Util.h"

using namespace std;

bool isClean(string s) {
    char c;

    for (int i = 0; i < s.length(); i++) {
        c = s.at(i);

        //checks for valid characters
        if ((c > 64 && c < 91) || (c > 96 && c < 123) || (c > 47 && c < 58)
            || c == 95 || c == 33 || c == 42 || c == 45 || c == 36
            || c == 63 || c == 37 || c == 38) {
            continue;
        }

        //if character isn't accpetable, reject
        else {
            return false;
        }

    }

    return true;
}

string hashString(string a, string b) {

    string result;
    hash<string> hashFunction;
    result = hashFunction(a);
    result += hashFunction(b);
    result = hashFunction(result);

    return result;
}

bool isValid(string s) {

    char c;
    bool hasUpper = false;
    bool hasLower = false;
    bool hasNumber = false;

    for (int i = 0; i < s.length(); i++) {

        c = s.at(i);

        if (c > 64 && c < 91) {
            hasUpper = true;
        }

        else if (c > 96 && c < 123) {
            hasLower = true;
        }

        else if (c > 47 && c < 58) {
            hasNumber = true;
        }

        else if (c == 95 || c == 33 ||
            c == 42 || c == 45 || c == 36 || c == 63 || c == 37 || c == 38) {

        }

        else {
            return false;
        }
    }

    return hasUpper && hasLower && hasNumber;

}

string generateRandomString(int length) {

    string result;

    srand((unsigned)time(NULL));

    for (int i = 0; i < length; i++) {
        result += (char)((rand() % 26) + 65);
    }

    return result;
}

bool isTaken(string userName) {

    //select count(*) from login where userName = userName
    // 
    // if (count == 1)
    //      return truel
    // else
    //      return false; 
    
    return false;
}

bool isValidID(int userID) {

    //select count(*) from login where userID = userID
    // 
    // if (count == 1)
    //      return truel
    // else
    //      return false; 
    return true;

}