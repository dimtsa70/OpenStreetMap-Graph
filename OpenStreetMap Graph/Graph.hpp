

#include "Vertex.hpp"

#include <vector>
#include <unordered_map>
#include <string>
using namespace std;

#ifndef _GRAPH_HPP_
#define _GRAPH_HPP_

//Klash Grafou
class Graph {
private:
    vector<Vertex> vertices;
    unordered_map<unsigned long, unsigned int> vertexMap;

    unordered_map<unsigned long, double> distances; //to pedio xrhsimopoieitai
                                                    //mono gia to shortest path
public:
    Graph();
    Graph(string& filename);
    Graph(const Graph& copyGraph);
    ~Graph();
    
    
    void addVertex(unsigned long id, double latitude, double longitude);
    
    void removeVertex(unsigned long id);

    void deleteGraph(); //diagrafh grafou se periptwsh prosthikhs allou
    
    list<unsigned long> shortestPath(unsigned long start, unsigned long end);
    void printShortestPath(unsigned long sid, unsigned long eid, list<unsigned long>& path);
    string formatCoordinate(double value); //h synarthsh kovei osa mhdenika yparxoun san 4o psifio stis syntetagmenes
    
    void compactGraph();

    void printGraphInfo() const;
    
    list<unsigned long> bfsPermeation(unsigned long sid);
    list<unsigned long> dfsPermeation(unsigned long sid);

    double haversineDistance(Vertex p1, Vertex p2); //ypologismos haversineDistance
    void deleteUnconnected();//diagrafh apomonwmenwn komvwn
    void printBfs_Dfs(list<unsigned long> bfsList);
    Graph& operator= (const Graph& graph); //yperfortwsh telesth = otan prostothetai allos grafos
};



#endif