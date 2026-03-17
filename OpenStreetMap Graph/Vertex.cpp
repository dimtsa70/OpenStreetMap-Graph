#include "Vertex.hpp"
#include <list>
#include <algorithm>


Vertex::Vertex(unsigned long id, double latitude, double longitude) {
    this->id = id;
    this->latitude = latitude;
    this->longitude = longitude;
}

Vertex::Vertex(const Vertex& copyVer) {
    this->id = copyVer.id;
    this->latitude = copyVer.latitude;
    this->longitude = copyVer.longitude;
    this->edgesList = copyVer.edgesList;
}

Vertex::Vertex() {
    
}
Vertex::~Vertex() {}

void Vertex::addEdge(Edge& newEdge) {
    this->edgesList.push_back(newEdge);
}


unsigned long Vertex::getId() {
    return this->id;   
}

double Vertex::getLat() {
    return this->latitude;
}

double Vertex::getLon() {
    return this->longitude;
}

int Vertex::edgeListSize() {
    if(edgesList.empty()) {
        return 0;
    }
    else {
        return edgesList.size();
    }
}

list<Edge>& Vertex::getEdgeList() {
    return this->edgesList;
}


void Vertex::removeEdgesConnectedTo(unsigned long otherId) {
    for (auto it = edgesList.begin(); it != edgesList.end(); ) {
        if (it->getStartPoint() == otherId || it->getDestPoint() == otherId) {
            it = edgesList.erase(it);
        } else {
            ++it;
        }
    
    }

}