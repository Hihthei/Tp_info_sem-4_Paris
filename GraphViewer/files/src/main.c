#include "graph.h"
#include "ShortestPath.h"

#include "display.h"

int main() {
	Graph* graph = jsonCreateGraphFromFile("./resources/graph.json");

	Path* path = Graph_shortestPath(graph, "0", "5");
	Path_print(path);

	displayGraphWindow(graph);

	Path_destroy(path);

	return 0;
}
