
#ifndef _EDGE_HPP_
#define _EDGE_HPP_

//Klash Akmwn
class Edge {
private:
    unsigned long idStartPoint;
    unsigned long idDestPoint;
    double distance;
   
    //prosthesi dyo epipleon pediwn ta opoia voithoun sto compact.
    bool oneWay;
    double factor;

public:    
    Edge(unsigned long idStartPoint, unsigned long idDestPoint, double distance, bool oneWay, double factor);
    Edge(const Edge& copyEdge);
    Edge();
    ~Edge();


    unsigned long getStartPoint();
    unsigned long getDestPoint();
    double getDistance();
    bool getOneWay();
    double getFactor();

    void setStartPoint(unsigned long idStartPoint);
    void setDestPoint(unsigned long idDestPoint);
    void setDistance(double distance);
    void setFactor(double factor);

};


#endif