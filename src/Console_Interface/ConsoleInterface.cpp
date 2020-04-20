#include "ConsoleInterface.h"


[[noreturn]] void ConsoleInterface::run() {

    while (true) {
        try{
            string command;
            cin >> command;
            string params;
            getline(cin, params);
            istringstream is(params);
            if(commandsMap.count(command)) {
                commandsMap[command]->execute(is);
            } else {
                cout << "Undefined command" << endl;
            }

        } catch (ExitException &exception) {
            cout << exception.what() << endl;
            stop();
        }
    }

}

void ConsoleInterface::stop() {
    remove(FIFO_FILE.c_str());
    pclose(gdb);
    exit(0);
}
