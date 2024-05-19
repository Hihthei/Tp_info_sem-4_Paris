#ifndef GRAPH_H
#define GRAPH_H

	#include "readJson.h"

	struct node {
		char* id;
		int data;
		float x;
		float y;
		struct nodesList* adjacent;
	};

	struct nodesList {
		struct node* node;
		int weight;
		struct nodesList* next;
	};

	struct Graph {
		int nodesCount;
		int oriented;
		char* fileName;
		struct nodesList* nodes;
	};

	typedef struct node Node;
	typedef struct nodesList NodesList;
	typedef struct Graph Graph;


	Graph* createGraph(typed(json_array)* nodesIdArray, const typed(json_element) jsonContent, const char* fileName);
	Graph* jsonCreateGraphFromFile(const char* jsonFile);
	void createAdjacentList(Graph* graph, NodesList* adj, char* adjacents[], long adjacentsWeight[], int nbElements);

	void graph_destroy();

	void saveGraph(Graph* graph);

	INLINE int Graph_size(Graph* graph)
	{
		assert(graph);
		return graph->nodesCount;
	}
	
	NodesList* Graph_getArcList(Graph* graph, int nodeIndex);

#endif