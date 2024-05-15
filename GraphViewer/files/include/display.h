#pragma once
#include "graph.h"
#include "raylib.h"

void displayGraphCLI(Graph* graph);
void displayGraphWindow(Graph* graph);

///////////////////////////////////////////
// 
//Graph preparation for drawing functions
//Get all information about nodes and edges selected that will be moved or edited
// 
///////////////////////////////////////////
void prepareGraphToDraw(Graph* graph, Vector2 startPoint,
	int* pressed, int* show, Rectangle* textBox,
	int* letterCount, char* edgeWeight, NodesList** editedEges, int* framesCounter, int* creatingNode,
	Node** movingNode, Vector2* mouse, int* createEdge);

void getUserInput(int* show, bool* edited, Rectangle* textBox, Vector2* mouse, int* letterCount, char* editedText, int* framesCounter, bool acceptChar);


void manageNodeRenameEditBox(Graph* graph, Node* node, int* show, Rectangle* textBox,
	Vector2* mouse, int* letterCount, char* editedText, int* framesCounter, bool* overNode, Node** nodeToEdit);

bool checkOverEdge(bool oriented, Node* node, Vector2* mouse, NodesList** editedEges);
void manageEdgeWeightEditBox(Graph* graph, Node* node, int* show, Rectangle* textBox,
	Vector2* mouse, int* letterCount, char* editedText, int* framesCounter, bool* overEdge, NodesList** editedEges);

bool deleteNodeManagement(Vector2* mouse, Node* node, NodesList* prevNodeList, NodesList* currentNodeList, Graph* graph);
bool deleteEdgeManagement(Graph* graph, Node* node, Vector2* mouse, NodesList** editedEges);

void createNodeManagement(int* creatingNode, Graph* graph, Vector2* mouse);

void edgeCreationManagement(Node** nodeToMove, Vector2* mouse, int* createEdge, Graph* graph);

///////////////////////////////////////////
// 
//Graph drawing functions
//Draw every element of the graph (edges, nodes, edge weights, etc.)
// 
///////////////////////////////////////////
void drawGraph(int show, Rectangle* textBox,
	char* name, int letterCount, int framesCounter,
	Vector2* currentPoint, Vector2* startPoint,
	Graph* graph, int moveStartPoint, Node* movingNode, int pressed,
	Vector2* mouse, int* createEdge);

void drawEdgeWeightEditBox(int show, Rectangle* textBox, char* edgeWeight, int letterCount, int framesCounter);
void drawOrientedCurve(Node* node, NodesList* adjencyList, Vector2* middle);