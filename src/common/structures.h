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

struct VertexWatchChanges{
    std::string oldValue;
    std::string newValue;
    std::string load;
    int vertex;

    VertexWatchChanges(std::string oldValue, std::string newValue, std::string load, int vertex):
            oldValue(oldValue), newValue(newValue), load(load), vertex(vertex){}
};

struct EdgeWatchChanges{
    std::string oldValue;
    std::string newValue;
    std::string load;
    int from, to;

    EdgeWatchChanges(std::string oldValue, std::string newValue, std::string load, int from, int to):
            oldValue(oldValue), newValue(newValue), load(load), from(from), to(to){}
};

struct VertexWatch{
    std::string load;
    int vertexIndex;

    VertexWatch(){};
    VertexWatch(std::string load, int vertexIndex): load(load), vertexIndex(vertexIndex){};
};

struct EdgeWatch{
    std::string load;
    int from, to;

    EdgeWatch(){};
    EdgeWatch(std::string load, int from, int to): load(load), from(from), to(to){};
};

struct VertexLoad{
    std::string variableName;
    std::string address;
    std::string type;

    VertexLoad(std::string variableName, std::string address, std::string type):
            variableName(variableName), address(address), type(type) {};
};

struct EdgeLoad{
    std::string variableName;
    std::string address;
    std::string type;

    EdgeLoad(std::string variableName, std::string address, std::string type):
            variableName(variableName), address(address), type(type) {};
};

inline bool operator<(const VertexLoad& a, const VertexLoad &b){
    return a.variableName < b.variableName;
}

inline bool operator<(const EdgeLoad& a, const EdgeLoad &b){
    return a.variableName < b.variableName;
}

struct Graph{
    std::vector<std::vector<int>> adjacencyMatrix;
    map<std::string, std::vector<std::string> > vertexLoads;
    map<std::string, std::vector<std::vector<std::string>>> edgeLoads;

    Graph() {};

    Graph(std::vector<std::vector<int>>& adjacencyMatrix, map<std::string, std::vector<std::string>>& vertexLoads, map<std::string, std::vector<std::vector<std::string>>>& edgeLoads):
            adjacencyMatrix(adjacencyMatrix), vertexLoads(vertexLoads), edgeLoads(edgeLoads){};
};

#endif //CORE_STRUCTURES_H
