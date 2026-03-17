
#ifndef _VERTEX_HPP_
#define _VERTEX_HPP_

#include "Edge.hpp"
#include <list>

using namespace std;

//Klash Vertex
class Vertex {
private:
    unsigned long id;
    double latitude;
    double longitude;
    list<Edge> edgesList;
public:
    Vertex(unsigned long id, double latitude, double longitude);

    Vertex(const Vertex& copyVer);
    Vertex();
    ~Vertex();

    void addEdge(Edge& newEdge);
    
    unsigned long getId();
    double getLat();
    double getLon();
    int edgeListSize();
    list<Edge>& getEdgeList();

    void removeEdgesConnectedTo(unsigned long otherId);

    
};





#endif