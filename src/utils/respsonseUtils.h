//
// Created by alimovdavron on 4/26/20.
//

#ifndef CORE_RESPSONSEUTILS_H
#define CORE_RESPSONSEUTILS_H
#include "../../json.hpp"
using json = nlohmann::json;

namespace responseUtils{
    json createBinaryResponse(bool success, std::string message){
        return json({
            {"success", success},
            {"message", message}
        });
    }

    std::string adjacencyMatrixToString(std::vector<std::vector<int>>& adjacencyMatrix){
        std::string result;
        for(const std::vector<int>& line: adjacencyMatrix){
            for(const int vertex: line){
                result += std::to_string(vertex) + " ";
            } result += "\n";
        }

        return result;
    }

    json createGraphResponse(bool success, std::vector<std::vector<int>> adjacencyMatrix){
        return json({
            {"success", success},
            {"graph", adjacencyMatrixToString(adjacencyMatrix)}
        });
    }
}

#endif //CORE_RESPSONSEUTILS_H
