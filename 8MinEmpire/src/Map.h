#ifndef MAP_H
#define MAP_H

#include "Player.h"

#include <queue>
#include <set>
#include <unordered_set>
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <set>

using namespace std;

class Player;
class Vertex;
typedef pair<Vertex*, bool> Edge;
typedef pair<string, string> PlayerEntry;

class Vertex {
    string *name;
    string *vertexKey;
    set<Player*> *owners;
    string *continent;
    unordered_map<PlayerEntry*, int> *armies;
    unordered_map<PlayerEntry*, int> *cities;
    vector<Edge> *edges;

public:
    Vertex();
    Vertex(string aName, string key, string continent);
    Vertex(Vertex* vertex);
    Vertex& operator =(Vertex& vertex);
    ~Vertex();

    void addEdge(Vertex* vertex, bool isWaterEdge);
    void print();
    string getRegionOwner();

    string getName(){return *name;}
    string getKey(){return *vertexKey;}
    string getContinent(){return *continent;}
    set<Player*>* getOwners(){return owners;}
    unordered_map<PlayerEntry*, int>* getArmies(){return armies;}
    unordered_map<PlayerEntry*, int>* getCities(){return cities;}
    vector<Edge>* getEdges(){return edges;}
};

typedef map<string, Vertex*> Vertices;

class GameMap {

private:
    static GameMap* mapInstance;
    Vertices* vertices;
    string* start;
    string* image;
    Vertex* startVertex;

public:
    GameMap(GameMap* map);
    ~GameMap();

    static GameMap* instance();

    void addVertex(const string& key, const string& name, const string& continent);
    void addEdge(const string& startVertex, const string& endVertex, const bool isWaterEdge);
    void printMap();
    void printOccupiedRegions();

    Vertices* getVertices() { return vertices; }
    string getStartVertexName() { return *start; }
    string getImage() { return *image; }
    Vertex* getStartVertex() { return startVertex; }

    void setImage(const string& newImage);
    bool setStartVertex(string& startVertexName);
    vector<set<string>* > getContinents();
    string getContinentOwner(set<string>* continent);

private:
    GameMap();
    string findOwnerOfContinent(unordered_map<string, int> *ownedRegionsPerPlayer);
};

#endif
