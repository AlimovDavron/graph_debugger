//
// Created by alimovdavron on 4/26/20.
//

#ifndef CORE_RESPONSEUTILS_H
#define CORE_RESPONSEUTILS_H
#include "../../json.hpp"
#include "../common/structures.h"

using json = nlohmann::json;

namespace responseUtils{
    json createBinaryResponse(bool success, std::string message){
        return json({
            {"success", success},
            {"message", message}
        });
    }

    json createStopResponse(std::string reason, Graph graph, Position position){
        return json({
            {"reason", reason},
            {"graph", json(graph.adjacencyMatrix)},
            {"position", {
                 {"line", position.line},
                 {"func", position.function},
                 {"file", position.file}
            }},
            {"loads", json(graph.vertexLoads)}
        });
    }

    json createWatchTriggerStopResponse(std::string reason, Graph graph, Position position, WatchChanges changes){
        return json({
            {"reason", reason},
            {"changes", {
                 {"vertex", changes.vertex},
                 {"load", changes.load},
                 {"old_value", changes.oldValue},
                 {"new_value", changes.newValues},
            }},
            {"graph", json(graph.adjacencyMatrix)},
            {"position", {
                 {"line", position.line},
                 {"func", position.function},
                 {"file", position.file}
            }},
            {"loads", json(graph.vertexLoads)}
        });
    }

    json createErrorStopResponse(std::string reason, std::string errorMessage, Graph graph, Position position){
        return json({
            {"reason", reason},
            {"error", errorMessage},
            {"graph", json(graph.adjacencyMatrix)},
            {"position", {
                 {"line", position.line},
                 {"func", position.function},
                 {"file", position.file}
            }},
            {"loads", json(graph.vertexLoads)}
        });
    }

    json createSetGraphResponse(bool success, Graph graph){
        return json({
            {"success", success},
            {"graph", json(graph.adjacencyMatrix)},
            {"loads", json(graph.vertexLoads)}
        });
    }
}

#endif //CORE_RESPONSEUTILS_H
