#include "Graph.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <iomanip>


using namespace std;

class Locality{
private:
    string title;
    size_t population;
public:

    Locality(const char* t, const size_t p): title(t),population(p){}

	Locality(const std::string t, const size_t p) : title(t), population(p) {}

	Locality(const Locality& obj) = delete;

	Locality& operator=(const Locality& obj) = delete;

	~Locality() {
		title.clear();
		population = 0;
	}

	Locality& operator=(Locality&& obj) {
		title = obj.title;
		obj.title.clear();
		population = obj.population;
		obj.population = 0;
	}

    std::string getTitle() const {
        return title;
    }

    size_t getPopulation() const {
        return population;
    }

    bool operator==(const Locality &rhs) const {
        return title == rhs.title &&
               population == rhs.population;
    }

    bool operator!=(const Locality &rhs) const {
        return !(rhs == *this);
    }
};

ostream& operator<<(ostream& out, Locality& locality){
    out<<locality.getTitle()<<":"<<locality.getPopulation();
    return out;
}



void printGraph(const Graph<Locality*, double>& graph) {
	for (auto i = 0; i < graph.GetVertexCount(); ++i) {
		cout << graph.GetVertex(i)->getTitle() << endl;
		for (auto j = 0; j < graph.GetEdgeCount(graph.GetVertex(i)); ++j) {
			cout << std::setw(5)<< "->" << graph.GetEdge(graph.GetVertex(i), j).dstVertex->getTitle() <<"("<< graph.GetEdge(graph.GetVertex(i), j).edge <<")"<< endl;
		}
		cout << endl;
	}
}

void createRandomEdges(Graph<Locality*, double> *graph) {
	auto count = graph->GetVertexCount();
	size_t v1 = 0;
	size_t v2 = 0;
	srand(time(NULL));
	for (auto i = 0; i < count*2; ++i) {
		v1 = rand() % count;
		v2 = rand() % count;
		if (v1 != v2) {
			graph->AddEdge(graph->GetVertex(v1), graph->GetVertex(v2), rand() % 7000 + 500);
		}
		else
			--i;
		}
}


int main() {
	   Graph<Locality*, double> roadNetwork;
	   Locality s("Samara", 1200000), m("Moscow", 17000000), sp("Saint-P", 12000000),v("Volgograd", 1000000),so("Sochi",450000),a("Abakan",300000),g("Grozny",1500000);

	   roadNetwork.AddVertex(&s);
	   roadNetwork.AddVertex(&m);
	   roadNetwork.AddVertex(&sp);
	   roadNetwork.AddVertex(&v);
	   roadNetwork.AddVertex(&so);
	   roadNetwork.AddVertex(&g);
	   roadNetwork.AddVertex(&a);
	
	   createRandomEdges(&roadNetwork);

	   cout << "Depth: ";
	   DepthFirstSearch(roadNetwork, &s, [](auto vertex) {
		   cout << vertex->getTitle() << "("<<vertex->getPopulation()<<")"<<endl;
		   });
	   cout << endl;

	   cout << "Breadth: " << endl;
	   BreadthFirstSearch(roadNetwork, &s, [](auto vertex) {
		   cout << vertex->getTitle() << "(" << vertex->getPopulation() << ")" << endl;;
		   });
	   cout << endl;

	   cout << "===================ORIGINAL ROAD NETWORK=================\n" << endl;
	   printGraph(roadNetwork);

	   roadNetwork.removeVertex(&s);

	   cout << "===================CHANGED ROAD NETWORK(without Samara)=================\n" << endl;
	   printGraph(roadNetwork);


	   return 0;
}
