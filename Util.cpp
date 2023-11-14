#include <iostream>
#include <functional>
#include<chrono>   //time for random seed
#include <queue>
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
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

bool isValidProductCode(int productCode) {

    string query0 = "select productCode from inventory where productCode = " + to_string(productCode);

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

bool isTaken(string userName) {

    if (!isClean(userName))
        return true;    //can't use an unclean name

    string query0 = "select userName from login where userName = '" + userName + "'";
    queue<string> result = query(1, query0);

    return result.size() > 0;
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

void showSQLError(unsigned int handleType, const SQLHANDLE& handle)
{
    SQLCHAR SQLState[1024];
    SQLCHAR message[1024];
    if (SQL_SUCCESS == SQLGetDiagRec(handleType, handle, 1, SQLState, NULL, message, 1024, NULL))
        // Returns the current values of multiple fields of a diagnostic record that contains error, warning, and status information
        cout << "SQL driver message: " << message << "\nSQL state: " << SQLState << "." << endl;
}

queue<string> query(int numOfItems, string inputQuery) {

    SQLHANDLE SQLEnvHandle = NULL;
    SQLHANDLE SQLConnectionHandle = NULL;
    SQLHANDLE SQLStatementHandle = NULL;
    SQLRETURN retCode = 0;
    const char *SQLQuery = inputQuery.c_str();
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
        switch (SQLDriverConnect(SQLConnectionHandle, NULL, (SQLCHAR*)"DRIVER={SQL Server}; SERVER=EngSQL01.franciscan.edu, 1433; DATABASE=University; UID=tabrown; PWD=76wZ99R@TubVvzf!;", SQL_NTS, retConString, 1024, NULL, SQL_DRIVER_NOPROMPT)) {
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