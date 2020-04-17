#include "ConsoleInterface.h"


[[noreturn]] void ConsoleInterface::run() {

    while (true) {
        string command;
        cin >> command;
        string params;
        getline(cin, params);
        istringstream is(params);
        commandsMap[command]->execute(is);
    }

}
