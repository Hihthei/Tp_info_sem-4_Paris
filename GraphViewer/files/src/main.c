#include <stdlib.h>
#include <stdio.h>

#include "graph.h"
#include "display.h"

int main() {
	printf("Mayssam");

	printf("message");
	Graph* graph = jsonCreateGraphFromFile("./resources/graph.json");
	displayGraphWindow(graph);
	return 0;
}
