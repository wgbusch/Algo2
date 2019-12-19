#include <vector>
#include <queue>
#include <stack>
#include <iostream>
#include <string>
#include <algorithm>


using namespace std;
/*
class UDS {
   private:
	vector<int> _padres;
	vector<int> _alturas;
	
   public:
	UDS(int n){
		for (int k = 1; k<=n; k++){
			_alturas[i] = 1;
			_padres[i] = 1;
		}
	}
	int padre(int i){
		return _padres[i];
	}
	int altura(int i){
		return _alturas[i];
	}
	int find(int x){
		if(padre(x) != x){
			_padres[x] = find(padre(x));
		}
		return _padres[x];
	}
	void union(int x, int y){
		int x = find(x);
		int y = find(y);
		
		if(altura(x) < altura(y)){
			_padres[x] = y;
		} else{
			_padres[y] =  x;
		}
		if(altura(x) == altura(y)){
			_alturas[x] = _alturas[x] +1;
		}
	}
}*/


// Representa un eje con su peso.
class Edge {
    public:
        int nodeA;
        int nodeB;
        int weight;

        Edge(int nodeA, int nodeB, int weight) {
            this->nodeA = nodeA;
            this->nodeB = nodeB;
            this->weight = weight;
        }
};


// Representa un grafo
class Graph {
	private:
		int _nodes; // La cantidad de nodos (son números consecutivos desde el 0)
		vector<Edge> _edges; // Una lista con los ejes

	public:
	    // Podemos construir un grafo con varios nodos ya de entrada.
		Graph(int amountNodes) {
		    this->_nodes = 0;

	        for(int i = 0; i < amountNodes; i++) {
	            this->addNode();
	        }
	    }

        int amountNodes() {
			return this->_nodes;
		}

		int amountEdges() {
		    return this->_edges.size();
		}

		void addNode() {
			this->_nodes += 1;
		}

		void addEdge(Edge edge) {
			this->_edges.push_back(edge);
		}

		vector<Edge> edges() {
		    return this->_edges;
		}
};


// Para encontrar la componente conexa a la que pertenece un nodo (DisjointSet)
// Recibe también un vector donde vamos guardando los padres de cada nodo.
int findComponent(int node, vector<int> &disjointSet) {


	return disjointSet[node];
}


// Para unir dos componentes conexas de dos nodos (DisjointSet)
// Recibimos vectores para almacenar info útil para el algoritmo y actualizarla: el peor eje de cada componente, el tamaño de las componentes, etc...
int unionComponent(int nodeA, int nodeB, vector<int> &disjointSet, vector<int> &componentsWorstEdge, int worstWeight, vector<int> &componentsSize) {
	int componentA = disjointSet[nodeA];
	int componentB = disjointSet[nodeB];
	int sizeA = componentsSize[nodeA];
	int sizeB = componentsSize[nodeB];
	int worstEdgeA = componentsWorstEdge[nodeA];
	int worstEdgeB = componentsWorstEdge[nodeB];
	int worst = min(-worstEdgeA,min(-worstEdgeB,-worstWeight));
	int sumOfSizes = sizeA + sizeB;

	for (int z = 0; z < disjointSet.size(); z++){
		if(disjointSet[z] == componentB ||
		   disjointSet[z] == componentA ){
			disjointSet[z] = componentA;
			componentsSize[z] = sumOfSizes;
			componentsWorstEdge[z] = worst;
		}
	}
}


// Función que sirve para comparar 2 ejes, útil para ordenar un vector de ejes.
bool compareEdges(Edge a, Edge b) {
    return a.weight < b.weight;
}


// Función que mide si dos componentes son similares
double mint(int nodeA, int nodeB, vector<int> &componentsWorstEdge, vector<int> &componentsSize, int k) {
    double a = componentsWorstEdge[nodeA] + ((double) k / componentsSize[nodeA]);
    double b = componentsWorstEdge[nodeB] + ((double) k / componentsSize[nodeB]);

    if(a < b) {
        return a;
    }

    return b;
}


vector<int> imageSegmentation(Graph &inputGraph, int k) {
    // El arbol y el grafo de entrada tienen la misma cantidad de nodos
    Graph outputGraph(inputGraph.amountNodes());

    // Inicializo el disjoint set y otras estructuras útiles
    vector<int> disjointSet(inputGraph.amountNodes(), -1);
    vector<int> componentsWorstEdge(inputGraph.amountNodes(), -1);
    vector<int> componentsSize(inputGraph.amountNodes(), 1);


    // Ordeno los ejes de más parecido a más distinto
    vector<Edge> inputEdges = inputGraph.edges();
    // TODO: Completar. Ordenar ejes
    ordenar(inputEdges);
    // Voy agregando las aristas en orden
    for(int i = 0; i < inputEdges.size(); i++) {
        // Obtengo las componentes de cada nodo
        int parentA = findComponent(inputEdges[i].nodeA, disjointSet);
        int parentB = findComponent(inputEdges[i].nodeB, disjointSet);

        // Si son de componentes conexas distintas
        // y además son "similares", los uno
        if(parentA != parentB) {
            // TODO: Completar. Agregar if para similaridad de componentes (ver función mint)
            if(mint(inputEdges[i].nodeA, inputEdges[i].nodeB, componentsWorstEdge, componentsSize, k)) {
                // Uno las dos componentes
                unionComponent(inputEdges[i].nodeA, inputEdges[i].nodeB, disjointSet, componentsWorstEdge, inputEdges[i].weight, componentsSize);
                outputGraph.addEdge(inputEdges[i]);
            }
        }
    }

    // Devolver la componente de cada nodo
    vector<int> components;
    for(int i = 0; i < outputGraph.amountNodes(); i++) {
        components.push_back(findComponent(i, disjointSet));
    }

    return components;
}


Graph readGraph() {
    int nodes, edges;
    cin >> nodes >> edges;

    Graph imageGraph(nodes);

    for(int i = 0; i < edges; i++) {
        int a, b, weight;
        cin >> a >> b >> weight;

        imageGraph.addEdge(Edge(a, b, weight));
    }

    return imageGraph;
}

void ordenar(vector<Edge> edges){
	return edges;
}


void writeGraph(vector<int> &components) {
    for(int i = 0; i < components.size(); i++) {
        cout << components[i] << ",";
    }
}


int main(int argc, char *argv[]) {
    int k;
    cin >> k;

    Graph inputGraph = readGraph();
    vector<int> components = imageSegmentation(inputGraph, k);
    writeGraph(components);
}
