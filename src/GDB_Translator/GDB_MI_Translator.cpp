//
// Created by alimovdavron on 10/29/19.
//

#include "GDB_MI_Translator.h"

std::string GDB_MI_Translator::flushGDBMIResponse() {
        int size;
        char buf[4096];
        std::string result;
        while((size = read(fileDescriptorFIFO, buf, sizeof(buf) - 1)) > 0){
            buf[size] = '\0';
            result += buf;
            if(has_suffix(result, "(gdb) \n")){
                return result;
            }
        }
}

std::vector<json> GDB_MI_Translator::readGDBMIResponse(std::string hash) {
    std::vector<json> actions;
    int size;
    char buf[4096];
    std::string response;
    while((size = read(fileDescriptorFIFO, buf, sizeof(buf) - 1)) > 0){
        buf[size] = '\0';
        response += buf;

        std::size_t pos = response.find("(gdb) \n");
        while (pos != std::string::npos){
            std::vector<std::string> outputs = split(response.substr(0, pos),
                                                     "\n");

            for(const auto& u: outputs){
                json action = outputParser->parseOutput(u);
                actions.push_back(action);
                if(action["token"] == hash){
                    return actions;
                }
            }

            response = response.substr(pos+7);
            pos = response.find("(gdb) \n");
        }
    }
}

std::vector<std::string> GDB_MI_Translator::split(std::string a, std::string delimiter) {
    std::vector<std::string> result;

    size_t pos = 0;
    std::string token;
    while ((pos = a.find(delimiter)) != std::string::npos) {
        token = a.substr(0, pos);
        // std::cout << token << std::endl;
        result.push_back(token);
        a.erase(0, pos + delimiter.length());
    }
    // std::cout << a << std::endl;
    result.push_back(token);
    return result;
}

json GDB_MI_Translator::runCommand(std::string command) {
    // TODO: generate hash
    std::string hash = "22";

    fprintf(gdb, "%s %s\n", hash.c_str(), command.c_str());
    fflush(gdb);

    std::vector<json> responseActions = readGDBMIResponse(hash);
    for(auto &u: responseActions){
        cout << u << endl;
    }
    return json();
}
