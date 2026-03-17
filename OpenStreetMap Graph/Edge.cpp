#include "Edge.hpp"


Edge::Edge(unsigned long idStartPoint, unsigned long idDestPoint, double distance, bool oneWay, double factor) {
    this->idStartPoint = idStartPoint;
    this->idDestPoint = idDestPoint;
    this->distance = distance;
    this->oneWay = oneWay;
    this->factor = factor;
}

Edge::Edge(const Edge& copyEdge) {
    this->idStartPoint = copyEdge.idStartPoint;
    this->idDestPoint = copyEdge.idDestPoint;
    this->distance = copyEdge.distance;
    this->oneWay = copyEdge.oneWay;
    this->factor = copyEdge.factor;
}

Edge::Edge() {
    
}

Edge::~Edge() {}
unsigned long Edge::getStartPoint() {
    return this->idStartPoint;
}

unsigned long Edge::getDestPoint() {
    return this->idDestPoint;
}

double Edge::getDistance() {
    return this->distance;
}

bool Edge::getOneWay() {
    return this->oneWay;
}

double Edge::getFactor() {
    return this->factor;
}

void Edge::setStartPoint(unsigned long idStartPoint) {
    this->idStartPoint = idStartPoint;
}

void Edge::setDestPoint(unsigned long idDestPoint) {
    this->idDestPoint = idDestPoint;
}

void Edge::setDistance(double distance) {
    this->distance = distance;
}

void Edge::setFactor(double factor) {
    this->factor = factor;
}