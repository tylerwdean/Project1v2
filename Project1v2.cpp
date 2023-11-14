#include "User.h"
#include "Util.h"
#include <queue>

int main() {

    //User currentUser = User();
    //currentUser.mainMenu();

    queue<string> result = query(2, "select * from instructor");
    string input;

    while (!result.empty()) {
        input = result.front();
        cout << input << endl;
        result.pop();
    }

    return 0;
}
