#include "display.h"

#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_CHARS     3


//Function to display the graph in the console
void displayGraphCLI(Graph* graph)
{
	NodesList* graphNodes = graph->nodes;
	while (graphNodes->next != NULL)
	{
		Node* node = graphNodes->node;
		printf("Node id: %s ", node->id);
		printf("(data: %d)\n", node->data);
		NodesList* adjencyList = node->adjacent;
		while (adjencyList != NULL)
		{
			printf("\t-Adjacent node id: %s\n", adjencyList->node->id);
			adjencyList = adjencyList->next;
		}
		graphNodes = graphNodes->next;
	}
}


//Function to display the graph in a window
void displayGraphWindow(Graph* graph)
{
	// Initialization
	const int screenWidth = 1200;
	const int screenHeight = 800;

	//Arbitrary starting point in the upper left corner of the window
	Vector2 startPoint = { 100, 100 };

	//Variable needed for moving nodes and creating edges
	Node* movingNode = NULL;
	int moveStartPoint = -1;
	bool moveEndPoint = false;
	int pressed = 0;

	int createEdge = 0;
	int creatingNode = 0;

	int displayEdgeWeightEditBox = 0;

	//Variables for text input
	char editedText[MAX_INPUT_CHARS + 1] = "\0";      // NOTE: One extra space required for null terminator char '\0'
	int letterCount = 0;

	Rectangle textBox = { 0, 0, 120, 50 };
	bool mouseOnText = false;

	int framesCounter = 0; //Variable for the blinking underscore


	Vector2 currentPoint = startPoint;
	Vector2 mouse;

	InitWindow(screenWidth, screenHeight, "Draw graphs");
	SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
	NodesList* editedEges;

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		currentPoint = startPoint;
		mouse = GetMousePosition();

		//Get all information needed to draw the graph
		//such user inputs, moving nodes, creating edges, etc.

		prepareGraphToDraw(graph, startPoint, &pressed,
			&displayEdgeWeightEditBox, &textBox, &letterCount, editedText, &editedEges,
			&framesCounter, &creatingNode, &movingNode,
			&mouse, &createEdge);

		// Draw the graph
		drawGraph(displayEdgeWeightEditBox, &textBox,
			editedText, letterCount, framesCounter, &currentPoint, &startPoint,
			graph, moveStartPoint, movingNode, pressed,
			&mouse, &createEdge);


		//Check if the user wants to save the graph
		if (IsKeyPressed(KEY_S))
		{
			saveGraph(graph);
		}
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseWindow();        // Close window and OpenGL context

	//--------------------------------------------------------------------------------------
}



void prepareGraphToDraw(Graph* graph, Vector2 startPoint,
	int* pressed, int* show, Rectangle* textBox,
	int* letterCount, char* editedText, NodesList** editedEges, int* framesCounter, int* creatingNode,
	Node** nodeToEdit, Vector2* mouse, int* createEdge)
{
	bool overEdge = false;
	bool overNode = false;
	NodesList* currentNodeList = graph->nodes;
	NodesList* prevNodeList = NULL;
	while (currentNodeList->next != NULL)
	{
		Node* node = currentNodeList->node;

		if (deleteNodeManagement(mouse, node, prevNodeList, currentNodeList, graph) || deleteEdgeManagement(graph, node, mouse, editedEges))
			break;

		prevNodeList = currentNodeList;

		createNodeManagement(creatingNode, graph, mouse);
		manageNodeRenameEditBox(graph, node, show, textBox, mouse, letterCount, editedText, framesCounter, &overNode, nodeToEdit);

		if (*nodeToEdit == NULL || *createEdge) {
			manageEdgeWeightEditBox(graph, node, show, textBox, mouse, letterCount, editedText, framesCounter, &overEdge, editedEges);



			//If the mouse is over a node, the user can move it by left clicking on it
			if (CheckCollisionPointCircle(*mouse, (Vector2) { node->x, node->y }, 50.0f)
				&& IsMouseButtonDown(MOUSE_BUTTON_LEFT))
				*nodeToEdit = node;

			if (*nodeToEdit != NULL)
			{
				edgeCreationManagement(nodeToEdit, mouse, createEdge, graph);

				//Once the user has moved the node, he can release it by releasing the left mouse button
				if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
					(*nodeToEdit) = NULL;
			}
			//If no node is being moved check the next one, but if a node is being moved, stop checking
			if ((*nodeToEdit) == NULL)
				currentNodeList = currentNodeList->next;
			else
				break;
		}
		else
			break;

	}
}


bool deleteNodeManagement(Vector2* mouse, Node* node, NodesList* prevNodeList, NodesList* currentNodeList, Graph* graph)
{
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && IsKeyDown(KEY_LEFT_ALT))
	{
		bool overNode = CheckCollisionPointCircle(*mouse, (Vector2) { node->x, node->y }, 50.0f);
		if (overNode)
		{
			if (prevNodeList)
				prevNodeList->next = currentNodeList->next;
			else
				graph->nodes = currentNodeList->next;

			NodesList* parseNodes = graph->nodes;
			while (parseNodes->next != NULL)
			{
				Node* curNode = parseNodes->node;
				if (curNode->adjacent != NULL)
				{
					NodesList* tmp = curNode->adjacent;
					NodesList* prevTmp = NULL;

					while (tmp != NULL)
					{
						if (tmp->node == node)
						{
							if (prevTmp != NULL)
								prevTmp->next = tmp->next;
							else
								curNode->adjacent = tmp->next;
							break;
						}
						prevTmp = tmp;
						tmp = tmp->next;
					}
				}
				parseNodes = parseNodes->next;
			}
			graph->nodesCount--;

			free(node->adjacent);
			free(node);
			return true;
		}
	}
	return false;
}



bool deleteEdgeManagement(Graph* graph, Node* node, Vector2* mouse, NodesList** editedEges)
{
	bool edgeRemoved = false;
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && IsKeyDown(KEY_LEFT_ALT))
	{
		bool overEdge = checkOverEdge(graph->oriented, node, mouse, editedEges);
		if (overEdge)
		{
			NodesList* prevAdjacent = NULL;
			NodesList* curAdjacent = node->adjacent;
			while (curAdjacent != NULL)
			{
				if (curAdjacent->node == (*editedEges)->node)
				{
					if (prevAdjacent != NULL)
						prevAdjacent->next = curAdjacent->next;
					else
						node->adjacent = curAdjacent->next;

					edgeRemoved = true;
					break;
				}
				prevAdjacent = curAdjacent;
				curAdjacent = curAdjacent->next;
			}
		}
	}

	return edgeRemoved;
}


void createNodeManagement(int* creatingNode, Graph* graph, Vector2* mouse)
{
	if (IsKeyDown(KEY_LEFT_CONTROL) && IsMouseButtonDown(MOUSE_BUTTON_LEFT) && *creatingNode == 0)
	{
		NodesList* curNodeList = graph->nodes;
		graph->nodesCount++;
		Node* nodeCurrent = (Node*)malloc(sizeof(Node));
		while (curNodeList->next->node != NULL)
		{
			curNodeList = curNodeList->next;
		}
		char buffer[10];
		_itoa(graph->nodesCount, buffer, 10);
		nodeCurrent->id = (char*)malloc(strlen(buffer));
		strcpy(nodeCurrent->id, buffer);
		nodeCurrent->data = (int)(mouse->x + mouse->y);
		nodeCurrent->x = mouse->x;
		nodeCurrent->y = mouse->y;
		nodeCurrent->adjacent = NULL;
		curNodeList->next->node = nodeCurrent;


		curNodeList->next->next = (NodesList*)malloc(sizeof(NodesList));
		curNodeList->next->next->node = NULL;
		curNodeList->next->next->next = NULL;

		*creatingNode = 1;
	}
	if (IsMouseButtonUp(MOUSE_BUTTON_LEFT))
		*creatingNode = 0;

}


void manageNodeRenameEditBox(Graph* graph, Node* node, int* show, Rectangle* textBox,
	Vector2* mouse, int* letterCount, char* editedText, int* framesCounter, bool* overNode, Node** nodeToEdit)
{
	int currentGesture = GetGestureDetected();

	if (!*show)
		*overNode = CheckCollisionPointCircle(*mouse, (Vector2) { node->x, node->y }, 50.0f);

	if ((currentGesture == GESTURE_DOUBLETAP && *overNode) || (*show && *nodeToEdit != NULL))
	{
		if (*nodeToEdit == NULL)
			*nodeToEdit = node;

		bool edited = false;
		getUserInput(show, &edited, textBox, mouse, letterCount, editedText, framesCounter, true);

		if (edited)
		{
			strcpy((*nodeToEdit)->id, editedText);
			strcpy(editedText, "");
			*letterCount = 0;
		}
	}
	else {
		if (!IsKeyDown(KEY_LEFT_SHIFT))
			*nodeToEdit = NULL;
	}
}


//If the graph is oriented, the function will check if the mouse is over the triangle indicated the orientation
//If the graph is not oriented, the function will check if the mouse is over anypart of the edge
bool checkOverEdge(bool oriented, Node* node, Vector2* mouse, NodesList** editedEges)
{
	bool overEdge = false;
	NodesList* adjencyList = node->adjacent;
	Vector3 start = { node->x, node->y,0 };
	while (adjencyList != NULL && adjencyList->node != NULL)
	{
		if (!oriented)
		{
			if (CheckCollisionPointLine(*mouse, (Vector2) { node->x, node->y }, (Vector2) { adjencyList->node->x, adjencyList->node->y }, 5))
			{
				overEdge = true;
				break;
			}
		}
		else {
			Vector3 end = { adjencyList->node->x, adjencyList->node->y,0 };
			Vector3 normal = Vector3Perpendicular(Vector3Subtract(end, start));
			Vector2 normalNormalized = Vector2Normalize((Vector2) { normal.x, normal.y });

			float multiplier = 30;
			normalNormalized.x *= multiplier;
			normalNormalized.y *= multiplier;

			Vector2 middle = { (node->x + adjencyList->node->x) / 2 + normalNormalized.x ,
				(node->y + adjencyList->node->y) / 2 + normalNormalized.y };

			Vector2 colinear = Vector2Normalize((Vector2) {
				(adjencyList->node->x - node->x) / 2,
					(adjencyList->node->y - node->y) / 2
			});
			colinear.x *= multiplier;
			colinear.y *= multiplier;
			Vector2 backVertice = Vector2Subtract(middle, colinear);

			multiplier = 0.2f;
			normalNormalized.x *= multiplier;
			normalNormalized.y *= multiplier;

			Vector2 triangle[3] = {
				middle,
				Vector2Add(backVertice, normalNormalized),
				Vector2Subtract(backVertice, normalNormalized)
			};

			if (CheckCollisionPointTriangle(*mouse, triangle[0], triangle[1], triangle[2]))
			{
				overEdge = true;

				break;
			}
		}
		adjencyList = adjencyList->next;
	}
	if (overEdge)
		*editedEges = adjencyList;

	return overEdge;
}


void getUserInput(int* show, bool* edited, Rectangle* textBox, Vector2* mouse, int* letterCount, char* editedText, int* framesCounter, bool acceptChar)
{
	if (!(*show)) {
		(*textBox).x = mouse->x;
		(*textBox).y = mouse->y;
	}
	*show = 1;

	// Set the window's cursor to the I-Beam
	SetMouseCursor(MOUSE_CURSOR_IBEAM);

	// Get char pressed (Unicode character) on the queue
	int key = GetKeyPressed();

	// Check if more characters have been pressed on the same frame
	while (key > 0)
	{
		// NOTE: Only allow numerci keys between 0...9
		if (((key >= KEY_ZERO) && (key <= KEY_NINE)) || ((key >= KEY_A) && (key <= KEY_Z) && acceptChar) && (*letterCount < MAX_INPUT_CHARS))
		{
			editedText[*letterCount] = (char)key;
			editedText[*letterCount + 1] = '\0'; // Add null terminator at the end of the string.
			(*letterCount)++;
		}
		else if (key == KEY_ENTER || key == KEY_KP_ENTER || key == KEY_ESCAPE)
		{
			*show = 0;
			SetMouseCursor(MOUSE_CURSOR_ARROW);
			if (key == KEY_ESCAPE)
				editedText[0] = '\0';
			*edited = true;
		}

		key = GetCharPressed();  // Check next character in the queue
	}

	if (IsKeyPressed(KEY_BACKSPACE))
	{
		(*letterCount)--;
		if (*letterCount < 0) *letterCount = 0;
		editedText[*letterCount] = '\0';
	}

	(*framesCounter)++;
	(*framesCounter) %= 100;
}

void manageEdgeWeightEditBox(Graph* graph, Node* node, int* show, Rectangle* textBox,
	Vector2* mouse, int* letterCount, char* editedText,
	int* framesCounter, bool* overEdge, NodesList** editedEges)
{
	int currentGesture = GetGestureDetected();

	if (!*show)
		*overEdge = checkOverEdge(graph->oriented, node, mouse, editedEges);

	if ((currentGesture == GESTURE_DOUBLETAP && *overEdge) || *show)
	{
		bool edited = false;
		getUserInput(show, &edited, textBox, mouse, letterCount, editedText, framesCounter, false);

		if (edited)
		{
			(*editedEges)->weight = atoi(editedText);
			strcpy(editedText, "");
			*letterCount = 0;
		}
	}
}


void edgeCreationManagement(Node** nodeToMove, Vector2* mouse, int* createEdge, Graph* graph)
{
	if (!IsKeyDown(KEY_LEFT_SHIFT))
	{
		(*nodeToMove)->x = mouse->x;
		(*nodeToMove)->y = mouse->y;
	}
	if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
	{
		if (*createEdge != 0)
		{
			NodesList* tmpNodeList = graph->nodes;
			while (tmpNodeList->node != NULL)
			{
				if (CheckCollisionPointCircle(*mouse, (Vector2) { tmpNodeList->node->x, tmpNodeList->node->y }, 50.0f))
				{
					NodesList* tmp = (*nodeToMove)->adjacent;
					while (tmp != NULL && tmp->next != NULL)
					{
						tmp = tmp->next;
					}
					if (tmp == NULL) {
						(*nodeToMove)->adjacent = (NodesList*)malloc(sizeof(NodesList));
						(*nodeToMove)->adjacent->node = tmpNodeList->node;
						(*nodeToMove)->adjacent->weight = 1;
						(*nodeToMove)->adjacent->next = NULL;
					}
					else
					{
						tmp->next = (NodesList*)malloc(sizeof(NodesList));
						tmp->next->node = tmpNodeList->node;
						tmp->next->weight = 1;
						tmp->next->next = NULL;
					}
					break;
				}
				tmpNodeList = tmpNodeList->next;
			}
		}
		*createEdge = 0;
	}
}

void drawGraph(int showEdgeWeightEditBox, Rectangle* textBox,
	char* edgeWeight, int letterCount, int framesCounter,
	Vector2* currentPoint, Vector2* startPoint,
	Graph* graph, int moveStartPoint, Node* movingNode, int pressed,
	Vector2* mouse, int* createEdge)
{
	BeginDrawing();

	ClearBackground(RAYWHITE);

	currentPoint = startPoint;
	int line = 0;
	int column = 0;
	NodesList* graphNodes = graph->nodes;
	while (graphNodes->next != NULL)
	{
		Node* node = graphNodes->node;
		if (node->x == -1 && node->y == -1)
		{
			node->x = currentPoint->x;
			node->y = currentPoint->y;
		}
		else
		{
			currentPoint->x = node->x;
			currentPoint->y = node->y;
		}
		NodesList* adjencyList = node->adjacent;
		while (adjencyList != NULL && adjencyList->node != NULL)
		{
			Vector2 middle = Vector2Add((Vector2) { node->x, node->y }, (Vector2) { adjencyList->node->x, adjencyList->node->y });
			middle = Vector2Scale(middle, 0.5);
			if (!graph->oriented)
				DrawLine((int)node->x, (int)node->y, (int)adjencyList->node->x, (int)adjencyList->node->y, GREEN);
			else
				drawOrientedCurve(node, adjencyList, &middle);

			DrawText(TextFormat("%d", adjencyList->weight), (int)middle.x, (int)middle.y, 30, RED);

			adjencyList = adjencyList->next;
		}


		column = (column + 1) % 5;
		if (column == 0)
			line++;
		node->x = currentPoint->x;
		node->y = currentPoint->y;

		currentPoint->x = startPoint->x + 200 * column;
		currentPoint->y = startPoint->y + 300 * line;


		graphNodes = graphNodes->next;
	}
	graphNodes = graph->nodes;
	while (graphNodes->next != NULL)
	{
		Node* node = graphNodes->node;

		if (node->x == -1 && node->y == -1)
		{
			node->x = currentPoint->x;
			node->y = currentPoint->y;
		}
		else
		{
			currentPoint->x = node->x;
			currentPoint->y = node->y;
		}
		DrawCircleV(*currentPoint,
			CheckCollisionPointCircle(*mouse, *currentPoint, 30.0f) && !IsKeyDown(KEY_LEFT_SHIFT) ? 50.f : 30.f,
			(moveStartPoint != -1 && movingNode == node) ? RED : BLUE);
		DrawText(TextFormat("%s", node->id), (int)currentPoint->x - 5, (int)currentPoint->y - 15, 30, WHITE);
		graphNodes = graphNodes->next;
	}

	if (IsKeyDown(KEY_LEFT_SHIFT) && movingNode != NULL)
	{
		*createEdge = 1;
		DrawLineEx((Vector2) { movingNode->x, movingNode->y }, * mouse, 3, BLACK);
	}

	if (showEdgeWeightEditBox)
		drawEdgeWeightEditBox(showEdgeWeightEditBox, textBox, edgeWeight, letterCount, framesCounter);

	EndDrawing();
}

void drawEdgeWeightEditBox(int show, Rectangle* textBox, char* edgeWeight, int letterCount, int framesCounter)
{
	DrawRectangleRec(*textBox, LIGHTGRAY);
	DrawRectangleLines((int)textBox->x, (int)textBox->y, (int)textBox->width, (int)textBox->height, RED);
	DrawText(edgeWeight, (int)textBox->x + letterCount * 5, (int)textBox->y + 8, 40, MAROON);


	if (letterCount < MAX_INPUT_CHARS)
	{
		// Draw blinking underscore char
		if (framesCounter > 50) DrawText("_", (int)textBox->x + 8 + MeasureText(edgeWeight, 40), (int)textBox->y + 12, 40, MAROON);
	}
	else DrawText("Press BACKSPACE to delete chars...", (int)textBox->x, (int)textBox->y - 20, 20, GRAY);
}

void drawOrientedCurve(Node* node, NodesList* adjencyList, Vector2* middle)
{
	Vector3 start = { node->x, node->y,0 };
	Vector3 end = { adjencyList->node->x, adjencyList->node->y,0 };
	Vector3 normal = Vector3Perpendicular(Vector3Subtract(end, start));
	Vector2 normalNormalized = Vector2Normalize((Vector2) { normal.x, normal.y });

	float multiplier = 30;
	normalNormalized.x *= multiplier;
	normalNormalized.y *= multiplier;

	*middle = (Vector2){ (node->x + adjencyList->node->x) / 2 + normalNormalized.x ,
		(node->y + adjencyList->node->y) / 2 + normalNormalized.y };

	Vector2 colinear = Vector2Normalize((Vector2) {
		(adjencyList->node->x - node->x) / 2,
			(adjencyList->node->y - node->y) / 2
	});
	colinear.x *= multiplier;
	colinear.y *= multiplier;
	//DrawLine(node->x, node->y, node->x + control1.x, node->y + control1.y, PURPLE);
	//DrawLine(adjencyList->node->x, adjencyList->node->y, 
	//	adjencyList->node->x + control1.x, adjencyList->node->y + control1.y, PURPLE);
	DrawSplineSegmentBezierCubic(
		(Vector2) {
		node->x, node->y
	},
		(Vector2) {
		node->x + normalNormalized.x, node->y + normalNormalized.y
	},
		(Vector2) {
		middle->x, middle->y
	},
		* middle,
		2,
		RED);
	DrawSplineSegmentBezierCubic(*middle, (Vector2) { middle->x, middle->y },
		(Vector2) {
		adjencyList->node->x + normalNormalized.x, adjencyList->node->y + normalNormalized.y
	},
		(Vector2) {
		adjencyList->node->x, adjencyList->node->y
	},
		2,
		RED);
	Vector2 backVertice = Vector2Subtract(*middle, colinear);

	multiplier = 0.2f;
	normalNormalized.x *= multiplier;
	normalNormalized.y *= multiplier;

	DrawTriangle(*middle,
		Vector2Add(backVertice, normalNormalized),
		Vector2Subtract(backVertice, normalNormalized), GREEN);
}



//Suppression d'un node
//Suppression d'un edge