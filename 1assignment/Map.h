#ifndef MAP_H
#define MAP_H

#include <queue>
#include <set>
#include <unordered_set>
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <set>

using namespace std;

struct Vertex {
    typedef pair<Vertex*, bool> Edge;

    string name;
    set<string> owner;
    string continent;
    unordered_map<string, int> armies;
    unordered_map<string, int> cities;
    vector<Edge> edges;

    Vertex(string aName, string continent) : name(aName), continent(continent) {}

    void addEdge(Vertex* vertex, bool isWaterEdge) {
        // cout << "Adding edge from " << name << " to " << vertex->name << endl;
        edges.push_back(Edge(vertex, isWaterEdge));
    }
};

typedef unordered_map<string, Vertex*> Vertices;

class GameMap {
    Vertices* vertices;
    string* start;

public:
    GameMap();
    ~GameMap();
    void addVertex(const string& name, const string& continent);
    void addEdge(const string& startVertex, const string& endVertex, const bool isWaterEdge);
    Vertices* getVertices();
    bool setStartVertex(const string& startVertex);
};

#endif
