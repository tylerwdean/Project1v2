#include <iostream>
#include <functional>
#include<chrono>   //time for random seed
#include <queue>
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
#include "Util.h"
#include <algorithm>
#include <conio.h>


using namespace std;

void showSQLError(unsigned int handleType, const SQLHANDLE& handle)
{
    SQLCHAR SQLState[1024];
    SQLCHAR message[1024];
    if (SQL_SUCCESS == SQLGetDiagRec(handleType, handle, 1, SQLState, NULL, message, 1024, NULL))
        // Returns the current values of multiple fields of a diagnostic record that contains error, warning, and status information
        cout << "SQL driver message: " << message << "\nSQL state: " << SQLState << "." << endl;
}
//don't bother understanding it, just follow the comments to use it
queue<string> query(int numOfItems, string inputQuery) {

    SQLHANDLE SQLEnvHandle = NULL;
    SQLHANDLE SQLConnectionHandle = NULL;
    SQLHANDLE SQLStatementHandle = NULL;
    SQLRETURN retCode = 0;
    const char* SQLQuery = inputQuery.c_str();
    queue<string> resultQueue;

    do {
        if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &SQLEnvHandle))
            // Allocates the environment
            break;

        if (SQL_SUCCESS != SQLSetEnvAttr(SQLEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
            // Sets attributes that govern aspects of environments
            break;

        if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, SQLEnvHandle, &SQLConnectionHandle))
            // Allocates the connection
            break;

        if (SQL_SUCCESS != SQLSetConnectAttr(SQLConnectionHandle, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0))
            // Sets attributes that govern aspects of connections
            break;

        SQLCHAR retConString[1024]; // Conection string
        switch (SQLDriverConnect(SQLConnectionHandle, NULL, (SQLCHAR*)"DRIVER={SQL Server}; SERVER=EngSQL01.franciscan.edu, 1433; DATABASE=Publix; UID=SirillaDean; PWD=FaithWithoutWork;", SQL_NTS, retConString, 1024, NULL, SQL_DRIVER_NOPROMPT)) {
            // Establishes connections to a driver and a data source
        case SQL_SUCCESS:
            break;
        case SQL_SUCCESS_WITH_INFO:
            break;
        case SQL_NO_DATA_FOUND:
            showSQLError(SQL_HANDLE_DBC, SQLConnectionHandle);
            retCode = -1;
            break;
        case SQL_INVALID_HANDLE:
            showSQLError(SQL_HANDLE_DBC, SQLConnectionHandle);
            retCode = -1;
            break;
        case SQL_ERROR:
            showSQLError(SQL_HANDLE_DBC, SQLConnectionHandle);
            retCode = -1;
            break;
        default:
            break;
        }

        if (retCode == -1)
            break;

        if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, SQLConnectionHandle, &SQLStatementHandle))
            // Allocates the statement
            break;

        if (SQL_SUCCESS != SQLExecDirect(SQLStatementHandle, (SQLCHAR*)SQLQuery, SQL_NTS)) {
            // Executes a preparable statement
            showSQLError(SQL_HANDLE_STMT, SQLStatementHandle);
            break;
        }
        else {
            char name[256];
            //char dept_name[256];
            while (SQLFetch(SQLStatementHandle) == SQL_SUCCESS) {
                // Fetches the next rowset of data from the result

                for (int i = 1; i <= numOfItems; i++) {
                    SQLGetData(SQLStatementHandle, i, SQL_C_DEFAULT, &name, sizeof(name), NULL);
                    resultQueue.push(name);
                }
                //SQLGetData(SQLStatementHandle, 1, SQL_C_DEFAULT, &name, sizeof(name), NULL);
                //SQLGetData(SQLStatementHandle, 2, SQL_C_DEFAULT, &dept_name, sizeof(dept_name), NULL);
                // Retrieves data for a single column in the result set
                //cout << name << " " << dept_name << endl;
            }
        }
    } while (FALSE);

    SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
    SQLDisconnect(SQLConnectionHandle);
    SQLFreeHandle(SQL_HANDLE_DBC, SQLConnectionHandle);
    SQLFreeHandle(SQL_HANDLE_ENV, SQLEnvHandle);

    return resultQueue;
}

string hashString(string a, string b) {

    string result;
    hash<string> hashFunction;
    result = to_string(hashFunction(a));
    result = to_string(hashFunction(b + result));

    return result;
}

bool isClean(string s) {
    char c;

    for (int i = 0; i < s.length(); i++) {
        c = s.at(i);

        //checks for valid characters
        if ((c > 63 && c < 91) || (c > 96 && c < 123) || (c > 47 && c < 58)
            || c == 95 || c == 33 || c == 42 || c == 45 || c == 36
            || c == 63 || c == 37 || c == 38 || c == 46 || c == 39) {
            continue;
        }

        //if character isn't accpetable, reject
        else {
            return false;
        }

    }

    return true;
}

bool isValidPassword(string s) {

    char c;
    bool hasUpper = false;
    bool hasLower = false;
    bool hasNumber = false;

    if (s.length() < 8) {
        return false;
    }

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

        else if (c == 95 || c == 33 || c == 42 || c == 45 || 
            c == 46 || c == 36 || c == 63 || c == 37 || c == 38 || c == 64 || c == 39) {

        }

        else {
            return false;
        }
    }

    return hasUpper && hasLower && hasNumber;

}

bool isValidProductCode(string productCode) {

    string query0 = "select productID from inventory where productID = " + productCode;

    queue<string> result = query(1, query0);

    return result.size() == 1;

}

string generateRandomString(int length) {

    string result;

    srand((unsigned)time(NULL));

    for (int i = 0; i < length; i++) {
        result += (char)((rand() % 26) + 65);
    }

    return result;
}

bool isTakenUsername(string userName) {

    if (!isClean(userName))
        return true;   

    if (userName.compare("quit") == 0)
        return true;

    string query0 = "select username from login where username = '" + changeApostraphe(userName) + "'";
    queue<string> result = query(1, query0);

    return !result.empty();
}

bool isTakenEmail(string email) {
    
    if (!isValidEmail(email))
        return false;

    string query0 = "select emailAddress from customer where emailAddress = '" + changeApostraphe(email) + "'";
    queue<string> result = query(1, query0);

    return !result.empty();
}

bool isTakenDiscountCode(string code) {
    queue<string> result = query(1, "select discountCode from discount where discountCode = '" + changeApostraphe(code) + "'");

    return result.size() > 0;
}

bool isValidEmployeeID(string userID) {

    if (!isValidID(stoi(userID)))
        return false;

    queue<string> role = query(1, "select employeeID from employee where employeeID = " + userID);

    return (!role.empty());
}

bool isValidID(int userID) {

    string query0 = "select userID from login where userID = " + to_string(userID);
    queue<string> result = query(1, query0);

    return result.size() == 1;
}

bool isValidOrder(int orderID) {

    string query0 = "select orderID from orders where orderID = " + to_string(orderID);
    queue<string> result = query(1, query0);

    return result.size() > 0;
}

bool isValidEmail(string email) {
    int found = email.find("@");
    int found2 = email.find(".", found);

    if (found != string::npos && found2 != string::npos && found2 != email.length()-1)
        return true;
    return false;
}

bool isValidPhoneNumber(string input) {
    return !(stoll(input) > 9999999999) || (stoll(input) < 1000000000);
}

string getPhoneNumber() {
    string phoneNumber = getNumberInRange(1000000000, 9999999999);
    return phoneNumber;
}

string getNumberInRange(long long low, long long high) {

    string input;
    bool enteredCorrectly = false;
    long long number;

    input = getLine();

    while (!enteredCorrectly) {
        if (isClean(input)) {
            try {
                number = stoll(input);
                if (number >= low && number <= high) {
                    enteredCorrectly = true;
                    continue;
                }
            }
            catch (...) {
                
            }
        }

        cout << "That's not a valid input.\n";
        cout << "Enter a value in the range " + to_string(low) + " to " + to_string(high) + ": ";
        input = getLine();
    }

    return input;
}

string getEmail() {

    string input;
    bool enteredCorrectly = false;
    input = getLine();

    while (!enteredCorrectly) {

        if (isClean(input) && isValidEmail(input)) {
            enteredCorrectly = true;
            continue;
        }

        cout << "That's not a valid email address.\n";
        cout << "Please enter a valid email address: \n";
        input = getLine();
    }

    return input;
}

string getLine() {

    string input;
    bool enteredCorrectly = false;

    cin >> input;
    while (!enteredCorrectly) {

        if (isClean(input)) {
            enteredCorrectly = true;
            continue;
        }

        cout << "That's not a valid input.\n";
        cout << "Please enter a valid input: ";
        cin >> input;
    }

    return input;
}

string changeApostraphe(string input) {

    for (int i = 0; i < input.length(); i++) {
        if (input.at(i) == '\'') {
            input.insert(i + 1, "\'");
            i++;
        }
    }

    return input;
}

string getDoubleInRange(double low, double high) {
    
    string input;
    bool enteredCorrectly = false;
    double number;

    input = getLine();

    while (!enteredCorrectly) {
        if (isClean(input)) {
            try {
                number = stod(input);
                if (number >= low && number <= high) {
                    enteredCorrectly = true;
                    continue;
                }
            }
            catch (...) {

            }
        }

        cout << "That's not a valid input.\n";
        cout << "Enter a value in the range " + to_string(low) + " to " + to_string(high) + ": ";
        input = getLine();
    }

    return input;
    
}

bool isValidDiscountCode(string discountCode) {

    queue<string> result = query(1, "select discountCode from discount where discountCode = '" + changeApostraphe(discountCode) + "'");
    return !result.empty();
}

string getDiscountCode() {

    string input;

    cout << "Do you have a discount code? y/n\n";
    input = getLine();

    if (input.compare("y") != 0)
        return "none";

    cout << "Please enter the discount code: ";
    input = getLine();

    while (!isValidDiscountCode(input)) {
        if (input.compare("none") == 0)
            return "none";
        cout << "That's not a valid discount code. Please try again or enter 'none' to continue without one.\n";
        input = getLine();
    }

    return input;
}

string getLineInvisible() {

    string output = "";
    char ch = _getch();

    while (ch != '\r') {
        
        if (ch == 8) {
            output = output.substr(0, output.length() - 1);
        }
        
        else {
            output += ch;
        }
        ch = _getch();

    }

    cout << "\n";

    return output;
}

void displayOrder(string orderID) {

    queue<string> order = query(2, "select productID, quantity from orderItems where orderID = " + orderID);
    queue<string> result;
    string query0;

    cout << "Order #" + orderID << "\n";
    cout << "-------------------------------\n";
    while (!order.empty()) {
        query0 = "select productName from inventory where productID = " + order.front();
        order.pop();
        result = query(1, query0);
        cout << result.front() << ", " << order.front() << "\n";
        order.pop();
    }

    cout << "-------------------------------\n";
    cout << "Total Cost: " + query(1, "select totalCost from orders where orderID = " + orderID).front() << "\n";

}

//displays the particular item
void displayItem(string productID) {

    queue<string> result = query(4, "select productName, price, quantityStocked, productID from inventory where productID = " + productID);

    cout << "\nName: " + result.front();
    result.pop();
    cout << "\nPrice: " + result.front();
    result.pop();
    cout << "\nQuantity Stocked: " + result.front() << "\n";
}