//
// Created by alimovdavron on 4/27/20.
//

#ifndef CORE_STRUCTURES_H
#define CORE_STRUCTURES_H

struct Position{
    int line;
    std::string file;
    std::string function;

    Position(int line, std::string file, std::string function):
            line(line), file(file), function(function){}
};

struct WatchChanges{
    std::string oldValue;
    std::string newValues;
    std::string load;
    int vertex;

    WatchChanges(std::string oldValue, std::string newValues, std::string load, int vertex):
            oldValue(oldValue), newValues(newValues), load(load), vertex(vertex){}
};

struct Graph{
    std::vector<std::vector<int>> adjacencyMatrix;
    map<std::string, std::vector<std::string> > vertexLoads;

    Graph() {};

    Graph(std::vector<std::vector<int>>& adjacencyMatrix, map<std::string, std::vector<std::string>>& vertexLoads):
            adjacencyMatrix(adjacencyMatrix), vertexLoads(vertexLoads){};
};

#endif //CORE_STRUCTURES_H
