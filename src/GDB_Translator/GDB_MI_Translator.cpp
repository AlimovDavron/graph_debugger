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

std::vector<json> GDB_MI_Translator::readGDMIResponseUntilHash(int hash) {
    std::vector<json> outputs;
    int size;
    char buf[4096];
    std::string response;
    while((size = read(fileDescriptorFIFO, buf, sizeof(buf) - 1)) > 0){
        buf[size] = '\0';
        response += buf;

        std::size_t pos = response.find("(gdb) \n");
        while (pos != std::string::npos){
            std::vector<std::string> records = split(response.substr(0, pos),
                                                     "\n");

            response = response.substr(pos+7);
            pos = response.find("(gdb) \n");

            bool foundHash = false;
            for(const auto& record: records){
                json parsedRecord = outputParser->parseOutput(record);
                outputs.push_back(parsedRecord);
                cout << parsedRecord << endl;
                if(parsedRecord["token"] == hash){
                    foundHash = true;
                }
            }

            if(foundHash) {
                return outputs;
            }
        }
    }
}

std::vector<json> GDB_MI_Translator::readGDMIResponseUntilStop() {
    std::vector<json> outputs;
    int size;
    char buf[4096];
    std::string response;
    while((size = read(fileDescriptorFIFO, buf, sizeof(buf) - 1)) > 0){
        buf[size] = '\0';
        response += buf;

        std::size_t pos = response.find("(gdb) \n");
        while (pos != std::string::npos){
            std::vector<std::string> records = split(response.substr(0, pos),
                                                     "\n");

            response = response.substr(pos+7);
            pos = response.find("(gdb) \n");

            bool foundStopTypeMessage = false;
            for(const auto& record: records){
                json parsedRecord = outputParser->parseOutput(record);
                outputs.push_back(parsedRecord);
                if(parsedRecord["type"] == "notify" && parsedRecord["message"] == "stopped"){
                    foundStopTypeMessage = true;
                }
            }

            if(foundStopTypeMessage){
                return outputs;
            }
        }
    }
}

std::vector<std::string> GDB_MI_Translator::split(std::string a, std::string delimiter) {
    std::vector<std::string> result;

    size_t pos = 0;
    std::string token;
    while ((pos = a.find(delimiter)) != std::string::npos) {
        token = a.substr(0, pos);
        result.push_back(token);
        a.erase(0, pos + delimiter.length());
    }
    return result;
}

std::vector<json> GDB_MI_Translator::executeCommand(std::string command, char mode) {
    if(mode == 'h') {
        int hash = rand() % 10000;

        fprintf(gdb, "%s %s\n", std::to_string(hash).c_str(), command.c_str());
        fflush(gdb);

        return readGDMIResponseUntilHash(hash);
    } else if(mode == 's'){
        fprintf(gdb, "%s\n", command.c_str());
        fflush(gdb);

        return readGDMIResponseUntilStop();
    }
}
