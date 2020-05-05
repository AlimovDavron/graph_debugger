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
            {"vertexLoads", json(graph.vertexLoads)},
            {"edgeLoads", json(graph.edgeLoads)}
        });
    }

    json createVertexWatchTriggerStopResponse(std::string reason, Graph graph, Position position, VertexWatchChanges changes){
        return json({
            {"reason", reason},
            {"changes", {
                  {"type", "vertex"},
                  {"vertex", changes.vertex},
                  {"load", changes.load},
                  {"old_value", changes.oldValue},
                  {"new_value", changes.newValue},
            }},
            {"graph", json(graph.adjacencyMatrix)},
            {"position", {
                  {"line", position.line},
                  {"func", position.function},
                  {"file", position.file}
            }},
            {"vertexLoads", json(graph.vertexLoads)},
            {"edgeLoads", json(graph.edgeLoads)}
        });
    }

    json createEdgeWatchTriggerStopResponse(std::string reason, Graph graph, Position position, EdgeWatchChanges changes){
        return json({
            {"reason", reason},
            {"changes", {
                {"type", "edge"},
                {"from", changes.from},
                {"to", changes.to},
                {"load", changes.load},
                {"old_value", changes.oldValue},
                {"new_value", changes.newValue},
             }},
            {"graph", json(graph.adjacencyMatrix)},
            {"position", {
                {"line", position.line},
                {"func", position.function},
                {"file", position.file}
             }},
            {"vertexLoads", json(graph.vertexLoads)},
            {"edgeLoads", json(graph.edgeLoads)}
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
            {"vertexLoads", json(graph.vertexLoads)},
            {"edgeLoads", json(graph.edgeLoads)}
        });
    }

    json createSetGraphResponse(bool success, Graph graph){
        return json({
            {"success", success},
            {"graph", json(graph.adjacencyMatrix)},
            {"vertexLoads", json(graph.vertexLoads)},
            {"edgeLoads", json(graph.edgeLoads)}
        });
    }
}

#endif //CORE_RESPONSEUTILS_H
