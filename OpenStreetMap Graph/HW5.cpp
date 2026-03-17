#include "Vertex.hpp"
#include "Edge.hpp"
#include "Graph.hpp"
#include "tinyxml2.h"
#include <iostream>
#include <stdio.h>
#include <limits>
using namespace std;

int main() {
    string choice;
    string filename;
    Graph mapGraph;
    unsigned long sid, eid;
    while(1) {
        /*cout << endl;
        cout << "-i <filepath>  : Import Graph from <filepath>";
        cout << endl;
        cout << "-c             : Compact Graph";
        cout << endl;
        cout << "-p <sid> <eid> : Estimate shortest path between start";
        cout << endl;
        cout << "                 node with <sid> and end node with <eid>";
        cout << endl;
        cout << "-b <sid>       : Print bfs starting from node with <sid>";
        cout << endl;
        cout << "-d <sid>       : Print dfs starting from node with <sid>";
        cout << endl;
        cout << "-q             : Exit wihout memory leaks";
        cout << endl;*/
        cout << "\nEnter your choice: " << endl;

        cin >> choice; 
        
        //eggrafh grafou
        if(choice.compare("-i") == 0) {
            cin >> filename;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            try
            {
                mapGraph = Graph(filename);
                cout << "Graph OK" << endl;
                continue;
            }
            catch(string ex) {
                cout << ex << endl;
            }
            
        }
        //symptiksi grafou
        else if(choice.compare("-c") == 0) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            mapGraph.compactGraph();
            cout << "Compact OK" << endl;
            
            continue;
        }

        //ektypwsh syntomoterou monopatiou me startId, endId
        else if(choice.compare("-p") == 0) {
            cin >> sid >> eid;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            list<unsigned long> path = mapGraph.shortestPath(sid, eid);
            mapGraph.printShortestPath(sid, eid, path);
            path.clear();
            continue;
        }

        //ektypwh bfs
        else if(choice.compare("-b") == 0) {
            cin >> sid;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            list<unsigned long> bfsList = mapGraph.bfsPermeation(sid);
            mapGraph.printBfs_Dfs(bfsList);
            bfsList.clear();
            continue;
        }

        //ektypwsh dfs
        else if(choice.compare("-d") == 0) {
            cin >> sid;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            list<unsigned long> dfsList = mapGraph.dfsPermeation(sid);
            mapGraph.printBfs_Dfs(dfsList);
            dfsList.clear();
            continue;
        }
        
        //termatismos programmatos
        else if(choice.compare("-q") == 0) {
            return(0);
        }
    
    
    }
}