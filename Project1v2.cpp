#include "User.h"
#include "Manager.h"
#include "Util.h"
#include <queue>

int main() {

    //query(1, "select username from login where userID = 6");
    //query(1, "SELECT userID FROM login WHERE password = '11772554993413179630'").front();
    User currentUser = User();
    currentUser.mainMenu();
    
    /*string query0 = "drop table TylerTest";
    string query1 = "create table TylerTest (firstName varchar(20), ID numeric(1,0) unique)";
    string query2 = "insert into TylerTest values ('Tyler', 7)";
    string query3 = "select firstName, ID from TylerTest";
    queue<string> result;
    query(0, query1);
    query(0, query2);
    result = query(2, query3);
    query(0, query0);
    
    for (int i = 1; !result.empty(); i++) {
        cout << result.front() << ". ";
        result.pop();
        if (i % 2 == 0)
            cout << endl;
    }*/

    /*
    char queryInput[256] = "select * from instructor";
    queue<string> resultQueue;
    resultQueue = query(2, queryInput);

    while (!resultQueue.empty()) {
        cout << resultQueue.front();
        resultQueue.pop();
    }
    */


    return 0;
}
