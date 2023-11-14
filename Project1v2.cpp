#include "User.h"
#include "Manager.h"
#include "Util.h"
#include <queue>

int main() {

    //Manager currentUser = Manager();
    //currentUser.mainMenu();
    
    string queryInput = "select * from instructor";
    queue<string> result;
    result = query(2, queryInput);
    cout << result.front();

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
