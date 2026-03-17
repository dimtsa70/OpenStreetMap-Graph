#include "Graph.hpp"
#include "tinyxml2.h"
#include "Vertex.hpp"
#include <list>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <queue>
#include <unordered_set>
#include <algorithm>
#include <stack>
#include <set>
#include <limits>
using namespace tinyxml2;
using namespace std;

Graph::Graph() {

}
Graph::Graph(string& filename) {
    XMLDocument doc;
    const char* filepath = filename.c_str();
    XMLError result = doc.LoadFile(filepath);
    Edge newEdge1;
    Edge newEdge2;
    double distance, factor = 1.0;
    std::vector<unsigned long> nodeRefs;
    
    //an den yparxei to arxeio petaei to string
    if(result == XML_ERROR_FILE_NOT_FOUND) {
        throw ("Unable to open file: " + filename);
    }
    //an den einai ths swsths morfhs ".osm" petaei ayto to string
    else if(result == XML_ERROR_PARSING_TEXT) {
        throw ("Invalid format for file: " + filename);
    }
    
    
    XMLElement* root = doc.FirstChildElement("osm");

    //prosthikh twn komvwn ston grafo
    for(XMLElement* node = root->FirstChildElement("node"); node; node = node->NextSiblingElement("node")) {
        unsigned long id = node->Unsigned64Attribute("id");
        double lat = node->DoubleAttribute("lat");
        double lon = node->DoubleAttribute("lon");
        addVertex(id, lat, lon);
    }

    //evresi akmwn kai ypologismos apostashs kathe akmis kai enthesi stous antistoixous komvous
    for(XMLElement* way = root->FirstChildElement("way"); way; way = way->NextSiblingElement("way")) {
        bool oneWay = false;
        bool highway = false;
        std::vector<unsigned long> nodeRefs;
        for (XMLElement* tag = way->FirstChildElement("tag"); tag; tag = tag->NextSiblingElement("tag")) {
            //factor = 1.0;
            const char* k = tag->Attribute("k");
            const char* v = tag->Attribute("v");
            if (!k || !v) continue;

            if(strcmp(k, "highway") == 0) {
                highway = true;
                if(strcmp(v, "motorway") == 0 || strcmp(v, "trunk") == 0) {
                    factor = 0.5;
                }
                else if(strcmp(v, "primary") == 0 || strcmp(v, "secondary") == 0) {
                    factor = 0.75;
                }
                else if(strcmp(v, "living_street") == 0 || strcmp(v, "unclassified") == 0) {
                    factor = 1.25;
                }
                else if(strcmp(v, "service") == 0 || strcmp(v, "track") == 0) {
                    factor = 1.5;
                }
                else {
                    factor = 1.0;
                }
            }
            
            if(strcmp(k, "oneway") == 0) {
                if(strcmp(v, "yes") == 0 || strcmp(v, "1") == 0) {
                    oneWay = true;
                }
            }
        }

        if (!highway) {
            continue; //an den einai highway, den mas endiaferei
        }
        
        //prosthiki komvwn pou syndeomtai se ena way
        for (XMLElement* nd = way->FirstChildElement("nd"); nd; nd = nd->NextSiblingElement("nd")) {
            unsigned long ref = nd->Unsigned64Attribute("ref");
            nodeRefs.push_back(ref);
        }

        for (size_t i = 0; i + 1 < nodeRefs.size(); i++) {
            unsigned long from = nodeRefs[i];
            unsigned long to = nodeRefs[i + 1];
            unsigned long idIndex, idIndex2;

            if(this->vertexMap.find(from) == this->vertexMap.end() ||
               this->vertexMap.find(to) == this->vertexMap.end()) {
                continue;
            }

            idIndex = this->vertexMap[from];
            idIndex2 = this->vertexMap[to];
            
            //ypologismos apostashs metaksi from, to
            distance = haversineDistance(this->vertices[idIndex], this->vertices[idIndex2]);
            distance = factor * distance;

            //edw prosthetw kai tis eiserxomenes akmes stous komvous
            //gia na ginei eukoloteros o ypologismos tou compact
            if(oneWay) {
                newEdge1 = Edge(from, to, distance, true, factor);
                this->vertices[idIndex].addEdge(newEdge1);
                this->vertices[idIndex2].addEdge(newEdge1);
            }
            else {
                newEdge1 = Edge(from, to, distance, false, factor);
                newEdge2 = Edge(to, from, distance, false, factor);
                this->vertices[idIndex].addEdge(newEdge1);
                this->vertices[idIndex].addEdge(newEdge2);
                this->vertices[idIndex2].addEdge(newEdge2);
                this->vertices[idIndex2].addEdge(newEdge1);
            }
        }
        nodeRefs.clear();
    }
    //diagrafh apomonwmenwn
    deleteUnconnected();
}

//copy-constructor pou xrisimopoieitai
//ston operator "=" gia yperfortwsh
Graph::Graph(const Graph& copyGraph) {
    this->vertices = copyGraph.vertices;
    this->vertexMap = copyGraph.vertexMap;
}

Graph::~Graph() {
    deleteGraph();
}

//prosthiki komvou ston grafo
void Graph::addVertex(unsigned long id, double lat, double lon) {
    Vertex vertex(id, lat, lon);
    this->vertices.push_back(vertex);
    this->vertexMap[id] = vertices.size() - 1;
}

//ypologismos apostashs
double Graph::haversineDistance(Vertex p1, Vertex p2) {
    double result;
    const int earthRadius = 6378137;
    double p1Lat = p1.getLat() * (M_PI/180);
    double p2Lat = p2.getLat() * (M_PI/180);
    double p1Lon = p1.getLon() * (M_PI/180);
    double p2Lon = p2.getLon() * (M_PI/180);
    
    double dF =  p1Lat - p2Lat;
    double dL = p1Lon - p2Lon;
    double sinDf = sin(dF/2);
    double sinDl = sin(dL/2);

    double a = pow(sinDf, 2) + cos(p1Lat) * cos(p2Lat) * pow(sinDl, 2);

    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    result = earthRadius * c;
    return(result);
}


void Graph::deleteUnconnected() {
    this->vertexMap.clear();
    for(vector<Vertex>::iterator it = vertices.begin(); it!= vertices.end();) {
        Vertex& v = *it;
        if(v.edgeListSize() == 0) {
            it = vertices.erase(it);
        }
        else {
            ++it;
        }

    }
    for(size_t i = 0; i < vertices.size(); i++) {
        this->vertexMap[vertices[i].getId()] = i;
    }
}

//bfs diaperash me start id to sid
list<unsigned long> Graph::bfsPermeation(unsigned long sid) {
   
    queue<unsigned long> q;
    list<unsigned long> result;

    if (vertexMap.find(sid) == vertexMap.end()) {
        return result;
    }
    unsigned long idIndex = this->vertexMap[sid];

    unordered_set<unsigned long> visited;
    
    q.push(sid);
    visited.insert(sid);

    while(!q.empty()) {
        unsigned long curr = q.front();
        q.pop();
        result.push_back(curr);
        idIndex = this->vertexMap[curr];
        Vertex currentVertex = this->vertices[idIndex];

        vector<unsigned long> neighbors;

        for(Edge e : currentVertex.getEdgeList()) {
            if(!e.getOneWay() || e.getStartPoint() == curr)
                neighbors.push_back(e.getDestPoint());
        }

        sort(neighbors.begin(), neighbors.end());
        
        for(unsigned long neighbor : neighbors) {
            if(visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                q.push(neighbor);
            }
        }
    }
    return result;
}

//dfs diaperash me start id to sid
list<unsigned long> Graph::dfsPermeation(unsigned long sid) {
    stack<unsigned long> st;
    list<unsigned long> result;

    if (vertexMap.find(sid) == vertexMap.end()) {
        return result;
    }
    unsigned long idIndex = this->vertexMap[sid];

    unordered_set<unsigned long> visited;
    st.push(sid);
    
    while(!st.empty()) {
        unsigned long curr = st.top();
        st.pop();
        visited.insert(curr);
        result.push_back(curr);
        
        idIndex = this->vertexMap[curr];

        Vertex currentVertex = this->vertices[idIndex];
        vector<unsigned long> neighbors;
        
        for(Edge e : currentVertex.getEdgeList()) {
            neighbors.push_back(e.getDestPoint());
        }

        sort(neighbors.rbegin(), neighbors.rend());

        for(unsigned long neighbor : neighbors) {
            if(visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                st.push(neighbor);
            }
        }
    }
    return result;

}

//symptiksi grafou
void Graph::compactGraph() {
    bool changed;
    do {
        changed = false;
        
        //gia kathe komvo pairnoume thn lista tou kai diaxwrizoume tis akmes se eiserxomenes
        //(inEdges) kai se eskerxomenes (outEdges)
        for(Vertex& v : vertices) {
            bool deleteV = false;
            list<Edge>& edges = v.getEdgeList();

            vector<Edge> inEdges, outEdges;
            for(Edge& e : edges) {
                if(v.getId() == e.getDestPoint()) {
                    inEdges.push_back(e);
                }
                if(v.getId() == e.getStartPoint()) {
                    outEdges.push_back(e);
                }
            }
            
            //gia mones kateuthinseis
            if(inEdges.size() == 1 && outEdges.size() == 1  && inEdges[0].getOneWay() && outEdges[0].getOneWay()) {
                
                unsigned long from = inEdges[0].getStartPoint();
                unsigned long to = outEdges[0].getDestPoint();
                if(from == to) {//se periptwsh poy einai o idios komvos
                    continue;
                }

                int mapFrom = vertexMap[from];
                int mapTo = vertexMap[to];
                double factor = inEdges[0].getFactor();

                //epanaypologismos apostashs
                double dist = haversineDistance(vertices[mapFrom], vertices[mapTo]);
                dist = factor * dist;
                
                //nea akmh
                Edge newEdge(from, to, dist, true, factor);

                //prosthesi akmhs
                vertices[mapFrom].addEdge(newEdge);
                vertices[mapTo].addEdge(newEdge);
                
                //afairesh endiamesou komvou
                deleteV = true;
                if(deleteV) {
                    removeVertex(v.getId());
                    changed = true;
                    continue;
                }

            }
            
            //gia diples kateuthinseis
            if(inEdges.size() == 2 && outEdges.size() == 2 && !inEdges[0].getOneWay() && !outEdges[0].getOneWay()) {
                
                //oi komvoi twn opoiwn oi akmes  eiserxontai ston endiameso
                unsigned long fromA = inEdges[0].getStartPoint();
                unsigned long fromB = inEdges[1].getStartPoint(); 
            
                //oi ekserxomenes akmes tou endiamesou
                unsigned long toA = outEdges[0].getDestPoint();
                unsigned long toB = outEdges[1].getDestPoint();

                //auto shmainei pws to synolo {fromA, fromB, toA, toB} periexei mono
                //dyo arithmous
                unordered_set<unsigned long> neighbors = {fromA, fromB, toA, toB};
                if(neighbors.size() != 2) {
                    continue;
                }            
                
                //evresi komvwn A,B (akrianwn)
                unordered_set<unsigned long>::iterator it1 = neighbors.begin();
                unordered_set<unsigned long>::iterator it2 = next(it1);
                unsigned long A = *it1;
                unsigned long B = *it2;

                int indexA = vertexMap[A];
                int indexB = vertexMap[B];
                double factor = inEdges[0].getFactor();
                

                //epanaypologismos apostashs
                double totalDistA_B = haversineDistance(vertices[indexA], vertices[indexB]);
                totalDistA_B = factor * totalDistA_B;
                
                //nees akmes
                Edge newEdge1(A, B, totalDistA_B, false, factor);
                Edge newEdge2(B, A, totalDistA_B, false, factor);

                //prosthiki newn akmwn
                vertices[indexA].addEdge(newEdge1);
                vertices[indexA].addEdge(newEdge2);

                vertices[indexB].addEdge(newEdge1);
                vertices[indexB].addEdge(newEdge2);

                //afairesh endiamesou komvou
                deleteV = true;
                if(deleteV) {
                    removeVertex(v.getId());
                    changed = true;
                    continue;
                }
            }
            if(!deleteV) {
                continue;
            }
        }   
    
    
    
    
    }while(changed);
    //printGraphInfo();
}

//Evresi shortest path
list<unsigned long> Graph::shortestPath(unsigned long start, unsigned long end) {
    
    unordered_map<unsigned long, unsigned long> previous; //prohgoumenos komvos
    unordered_set<unsigned long> visited; //komvoi pou exei episkeftei h synarthsh

    using State = pair<double, unsigned long>; //Zeugari distance, vertexid
    
    auto cmp = [](const State& a, const State& b) { //comparator pou lamvanei ypopsin
        if(a.first!= b.first) {                     //poia einai h mikroterh apostash h anagnwristiko komvou
            return a.first > b.first;
        }
        else {
            return a.second > b.second;
        }
    };

    priority_queue<State, vector<State>, decltype(cmp)> pq(cmp); //priority queue

    for(Vertex& v : vertices) {
        distances[v.getId()] = numeric_limits<double>::infinity(); //thesimo pediou distances me infinity
    }

    distances[start] = 0.0;
    pq.push({0.0, start});

    //oso h oura den einai adeia
    //ginontai oi aparaithtes energeies
    //gia thn evresi ths mikroterhs apostashs

    while(!pq.empty()) {
        pair<double, unsigned long> top = pq.top(); //pairnoume ton mikrotero se apostasi komvo

        unsigned long currentId = top.second;
        pq.pop();

        if(visited.count(currentId)) {
            continue;
        }
        visited.insert(currentId);

        if(currentId == end) {//telos monopatiou
            break;
        }
        int vertexId = vertexMap[currentId];
        Vertex& currentVertex = vertices[vertexId];

        for(Edge& e : currentVertex.getEdgeList()) {
            if(e.getStartPoint() != currentId) {//theloume mono tis ekserxomenes akmes na elegksoume
                continue;
            }
            unsigned long neighborId = e.getDestPoint();
            double alt = distances[currentId] + e.getDistance();
            
            //an h apostash alt < infinity 'h an einai idies alla me mikrotero anagnwristiko
            //o currentId tote mpainei sthn oura o neighborId
            if (alt < distances[neighborId] || (abs(alt - distances[neighborId]) < 1e-9 && currentId < neighborId)) {
                distances[neighborId] = alt;
                previous[neighborId] = currentId;
                pq.push({alt, neighborId});
            }
        }
    }

    //eggrafh tou syntomoterou monopatiou
    list<unsigned long> path;
    if(distances[end] == numeric_limits<double>::infinity()) {
        return path;
    }
    //anapodh eggrafh
    for (unsigned long at = end; at != start; at = previous[at]) {
        path.push_front(at);
    }
        
    path.push_front(start);
    
    return path;
}

//ektypwsh syntomoterou monopatiou me url
void Graph::printShortestPath(unsigned long sid, unsigned long eid, list<unsigned long>& path) {
    if(path.empty()) {
        cout << "No shortest path found" << endl;
        return;
    }

    double dist = 0.0;
    list<unsigned long>::iterator it = path.begin();
    unsigned long prev = *it;
    ++it;
    
    while(it != path.end()) {
        unsigned long curr = *it;
        dist = distances.at(curr);
        cout << "[" << prev << " -> " << curr << "] ";
        cout << std::fixed << std::setprecision(3) << dist << endl;
        prev = curr;
        ++it;
    }

    cout << "\nhttps://www.google.com/maps/dir/";
    cout.unsetf(std::ios_base::fixed);
    
    for(unsigned long id : path) {
        Vertex& v = vertices[vertexMap[id]];

        cout << formatCoordinate(v.getLat()) << "," << formatCoordinate(v.getLon()) << "/"; //gia psalidwma mhdenikwn pou vriskontai
                                                                                            //sto teleutaio dekadiko psifio
    }
    cout << endl;
    this->distances.clear();
}


string Graph::formatCoordinate(double value) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(4) << value;
    std::string str = oss.str();

    
    str.erase(str.find_last_not_of('0') + 1);

   
    if (str.back() == '.')
        str.pop_back();

    return str;
}

//ektypwsh bfs_dfs
void Graph::printBfs_Dfs(list<unsigned long> List) {
    for(unsigned long id : List) {
        cout << id << endl;
    }
}

//afairesh komvou
void Graph::removeVertex(unsigned long id) {
    unsigned int index = vertexMap[id];
    //afairesh akmwn pou aforoun ton komvo auto
    for(Vertex& v : vertices) {
        v.removeEdgesConnectedTo(vertices[index].getId());
    }
    
    //ekkatharish listas komvou pros diagrafh
    vertices[index].getEdgeList().clear();
    
    //diagrafh komvou
    swap(vertices[index], vertices.back());
    vertices.pop_back();
    vertexMap.erase(id);

    vertexMap[vertices[index].getId()] = index;
}


//diagrafh grafou
void Graph::deleteGraph() {
     this->vertexMap.clear();
    for(Vertex& v : vertices) {
        v.getEdgeList().clear();
    }          
    this->vertices.clear();
    this->distances.clear();
}    

//yperfortwsh telesth otan prostithetai allos grafos
Graph& Graph::operator= (const Graph& graph)  {
    if(this->vertices.size() != 0 && this->vertexMap.size() != 0) {
        deleteGraph();
    }
    this->vertices = graph.vertices;
    this->vertexMap = graph.vertexMap;
    return *this;   
}



void Graph::printGraphInfo() const {
    cout << "Vertices:\n";
    for ( auto vertex : vertices) {
        cout << "ID: " << vertex.getId()
             << ", Lat: " << vertex.getLat()
             << ", Lon: " << vertex.getLon() << endl;
    }

    cout << "\nEdges:\n";
    for (auto vertex : vertices) {
        // unsigned long from_id = vertex.getId();
        for (auto edge : vertex.getEdgeList()) {
            if (edge.getDestPoint() == vertex.getId()) {
                continue; // Skip edges that point to the same vertex
            }
            cout << "From: " << edge.getStartPoint()
                 << " -> To: " << edge.getDestPoint()
                 << ", Distance: " << edge.getDistance() << endl;
        }
    }
}