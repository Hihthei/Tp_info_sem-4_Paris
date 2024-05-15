#include "graph.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>





Graph* createGraph(typed(json_array)* nodesIdArray, const typed(json_element) jsonContent, const char* fileName)
{
	Graph* graph = (Graph*)malloc(sizeof(Graph));
	graph->nodesCount = (int)nodesIdArray->count;

	NodesList* nodesList = (NodesList*)malloc(sizeof(NodesList));
	graph->nodes = nodesList;

	//For the number of nodescount, we create a node and add it to the list
	//of nodes of the graph, 
	// each node has an empty adjacent list
	for (int i = 0; i < graph->nodesCount; i++)
	{
		Node* nodeCurrent = (Node*)malloc(sizeof(Node));
		typed(json_element) nodeId = nodesIdArray->elements[i];
		int nameLength = strlen(nodeId.value.as_string);
		nodeCurrent->id = (char*)malloc(nameLength);
		strcpy(nodeCurrent->id, nodeId.value.as_string);
		nodeCurrent->data = i * 10;
		nodeCurrent->x = -1;
		nodeCurrent->y = -1;

		nodeCurrent->adjacent = NULL;
		nodesList->node = nodeCurrent;
		nodesList->next = (NodesList*)malloc(sizeof(NodesList));
		nodesList = nodesList->next;
		nodesList->node = NULL;
		nodesList->next = NULL;
	}

	graph->fileName = (char*)malloc(strlen(fileName));
	typed(json_element) bOriented = getJsonElementFromName(jsonContent, "oriented");
	graph->oriented = bOriented.value.as_number.value.as_long;
	strcpy(graph->fileName, fileName);

	return graph;
}


Graph* jsonCreateGraphFromFile(const char* jsonFile)
{
	//json structure : 
	//nbNodes is an integer containing the number of nodes
	//oriented is a boolean containing if the graph is oriented or not
	//each node is a dictionary whose key is its id and its value contains its coordinates {x, y} and its adjencyList
	//The adjencyList is an n-element array of 2-elements array : first element is the id of the connected node and the second is the weight of the edge
	typed(json_element) jsonContent = interpretJSon(jsonFile);
	typed(json_element) nodesIds = getJsonElementFromName(jsonContent, "nodesIds");

	typed(json_array)* nodesIdArray = nodesIds.value.as_array;
	Graph* graph = createGraph(nodesIdArray, jsonContent, jsonFile);


	// Get the nodes from the graph and fill the information from the json file
	NodesList* curNodeList = graph->nodes;
	for (int i = 0; i < graph->nodesCount; i++)
	{
		//retrieve the node id
		//With its id, get the dictionary containing the node information
		typed(json_element) nodeId = nodesIdArray->elements[i];
		typed(json_element) nodeAsObject = getJsonElementFromName(jsonContent, nodeId.value.as_string);

		//From this dictionnary, get the x and y coordinates of the node
		Node* curNode = curNodeList->node;
		curNode->x = -1;
		typed(json_element) nodeX = getJsonElementFromName(nodeAsObject, "x");
		curNode->x = (float)nodeX.value.as_number.value.as_double;

		curNode->y = -1;
		typed(json_element) nodeY = getJsonElementFromName(nodeAsObject, "y");
		curNode->y = (float)nodeY.value.as_number.value.as_double;

		//From this dictionnary, get the adjency list of the node
		if (hasElementWithName(nodeAsObject, "adjencyList"))
		{

			typed(json_element) connectedNodeIds = getJsonElementFromName(nodeAsObject, "adjencyList");

			typed(json_array)* values = connectedNodeIds.value.as_array;
			char** adjencents = malloc(sizeof(char*) * values->count);
			long* adjencentsWeight = malloc(sizeof(long) * values->count);
			//For each element of the adjency list, get the id of the connected node and the weight of the edge
			for (int j = 0; j < values->count; j++) {
				typed(json_element) element = values->elements[j];
				typed(json_array)* edgeInfos = element.value.as_array;
				if (edgeInfos != NULL)
				{
					if (edgeInfos->elements[0].value.as_number.value.as_long != -1) {
						adjencents[j] = (char*)malloc(strlen(edgeInfos->elements[0].value.as_string));
						strcpy(adjencents[j], edgeInfos->elements[0].value.as_string);
						adjencentsWeight[j] = edgeInfos->elements[1].value.as_number.value.as_long;
					}
				}
			}

			NodesList* adjencyList = (NodesList*)malloc(sizeof(NodesList));
			curNode->adjacent = adjencyList;
			adjencyList->node = NULL;
			adjencyList->next = NULL;
			createAdjacentList(graph, adjencyList, adjencents, adjencentsWeight, (int)values->count);
		}
		curNodeList = curNodeList->next;

	}

	//json_print(&element, 2);
	//json_free(&element);
	return graph;
}

void createAdjacentList(Graph* graph,
	NodesList* adjencyList,
	char* adjacents[],
	long adjacentsWeight[],
	int nbElement)
{
	//look in graphe for node 2, 3 and 4 because we have 
	// an edge between node 1 and 2, 3 and 4
	for (int i = 0; i < nbElement; i++)
	{
		NodesList* tmp = graph->nodes;
		//If vertexNode is not NULL, we have to create the next element of the list
		//and set this new element as the next of current adjacent element
		if (adjencyList->node != NULL)
		{
			adjencyList->next = (NodesList*)malloc(sizeof(NodesList));
			adjencyList = adjencyList->next;
			adjencyList->weight = 1;
			adjencyList->node = NULL;
			adjencyList->next = NULL;
		}

		//look for the node in the graph
		while (tmp != NULL)
		{
			if (tmp->node != NULL && strcmp(tmp->node->id, adjacents[i]) == 0)
			{
				adjencyList->node = tmp->node;
				adjencyList->weight = adjacentsWeight[i];
				break;
			}
			tmp = tmp->next;
		}
	}
}

void saveGraph(Graph* graph)
{
	FILE* file = fopen(graph->fileName, "w");
	if (file == NULL)
	{
		fprintf(stderr, "Error opening file %s\n", graph->fileName);
		return;
	}
	fprintf(file, "{");
	fprintf(file, "\"oriented\":%d,", graph->oriented);
	//Saves the nodes ids
	fprintf(file, "\"nodesIds\":[");
	NodesList* curNodeList = graph->nodes;
	for (size_t i = 0; i < graph->nodesCount; i++)
	{
		Node* curNode = curNodeList->node;
		fprintf(file, "\"%s\"", curNode->id);
		curNodeList = curNodeList->next;
		if (i != graph->nodesCount - 1)
			fprintf(file, ",");
	}
	fprintf(file, "],");

	//Saves each node information	
	curNodeList = graph->nodes;
	for (size_t i = 1; i <= graph->nodesCount; i++)
	{
		Node* curNode = curNodeList->node;
		fprintf(file, "\"%s\":{\"x\":%.1f,\"y\":%.1f,\"adjencyList\":[", curNode->id, curNode->x, curNode->y);
		NodesList* adjencyList = curNode->adjacent;
		while (adjencyList != NULL && adjencyList->node != NULL)
		{
			fprintf(file, "[\"%s\",%d]", adjencyList->node->id, adjencyList->weight);
			adjencyList = adjencyList->next;
			if (adjencyList != NULL)
				fprintf(file, ",");
		}
		fprintf(file, "]}");
		if (i != graph->nodesCount)
			fprintf(file, ",");
		curNodeList = curNodeList->next;
	}
	fprintf(file, "}");
	fclose(file);
}
