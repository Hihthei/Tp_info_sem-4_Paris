#pragma once
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
void saveGraph(Graph* graph);