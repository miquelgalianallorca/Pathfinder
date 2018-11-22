#include <stdafx.h>
#include <algorithm>
#include "path.h"

Path::~Path()
{
	for (Node* node : nodes)
		delete node;
	nodes.clear();
	path.clear();
}

void Path::Load(const std::string& filename, const unsigned int rows, const unsigned int cols)
{
	this->rows = rows;
	this->cols = cols;

	// Clear previous nodes
	for (Node* node : nodes)
	{
		delete node;
	}
	nodes.clear();
	
	// Read file
	std::ifstream istream(filename, std::ios_base::binary);
	std::stringstream sstream;
	sstream << istream.rdbuf();
	mapSrc = sstream.str();
	// Sanitize
	mapSrc.erase(std::remove(mapSrc.begin(), mapSrc.end(), '\n'), mapSrc.end());
	mapSrc.erase(std::remove(mapSrc.begin(), mapSrc.end(), '\r'), mapSrc.end());

	// Get nodes from map
	for (size_t i = 0; i < mapSrc.length(); ++i)
	{
		Node* node = new Node();
		node->cost = static_cast<int>(mapSrc.at(i)) - static_cast<int>('0');
		node->posY = i / cols;
		node->posX = i % cols;
		node->parent = nullptr;
		nodes.push_back(node);
	}
}

void Path::ResetNodes()
{
	for (Node* node : nodes)
	{
		node->parent = nullptr;
		node->g = 0.f;
		node->f = 0.f;
	}
}

// Heuristics: Euclidean distance. Manhattan could give longer paths than necessary.
// Start and end in map coords (not in screen coords)
bool Path::AStar(const float startX, const float startY, const float endX, const float endY)
{
	ResetNodes();

	/*
	openlist
	closedlist
	add nodeInicio a openlist, coste 0
	while not openlist.vacio
		nodo = openlist.pop_shortest()
		if isGoal(nodo)
			return buildPath(nodo)
		else
			for nextNode : conexiones(node)
				if nextNode in closedlist
					continue
				if nextNode in openlist
					update cost in openlist if smaller
				else
					add to openlist con padre node

	buildPath(pathnode)
		while(padre(pathNode))
			add node(pathNode) to path
		return path
	*/

	cout << "Start: " << startX << " " << startY << endl;
	cout << "End: "   << endX   << " " << endY   << endl;
	cout << endl;
	
	std::list<Node*> openList;
	std::list<Node*> closedList;

	// openList.push_back(Node(static_cast<unsigned int>(startX), static_cast<unsigned int>(startY), 0));
	Node* startNode = GetNodeAtPosition(static_cast<unsigned int>(startX),
		static_cast<unsigned int>(startY));
	openList.push_back(startNode);

	while (openList.size() > 0)
	{
		// Order by shortest
		openList.sort(Path::OrderByShortest);
		// Get shortest
		Node* node = openList.front();
		openList.pop_front();

		// Check goal
		if (node->posX == endX && node->posY == endY)
		{
			BuildPath(node);
			return true;
		}

		// Connections
		std::list<Node*> connections = GetConnections(node->posX, node->posY);
		for (Node* next : connections)
		{
			// Connection is in closed list
			auto it = std::find(closedList.begin(), closedList.end(), next);
			if (it != closedList.end())
			{
				continue;
			}

			// Connection is in open list
			it = std::find(openList.begin(), openList.end(), next);
			if (it != openList.end())
			{
				// Update cost in open list if smaller
				int newCost = node->cost + next->cost;
				if (newCost < (*it)->cost)
				{
					(*it)->cost = newCost;
				}
			}
			// Add to openlist with node as parent
			else
			{
				next->parent = node;
				openList.push_back(next);
				closedList.push_back(node);
			}
		}
	}

	return false;
}

void Path::DrawDebug(const size_t& squareSize)
{
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();
	
	for (const Node* node : nodes)
	{
		if (node->cost > 0)
		{
			float alpha = (node->cost) / 10.f;
			gfxDevice.SetPenColor(alpha + .3f, 0.f, 0.f, .1f);

			float wPosX, wPosY;
			CoordToWorldPos(node->posX, node->posY, squareSize, wPosX, wPosY);
			MOAIDraw::DrawRectFill(wPosX, wPosY, wPosX + squareSize*2, wPosY + squareSize*2);
		}
	}
}

void Path::CoordToWorldPos(const unsigned int InPosX, const unsigned int InPosY, const size_t squareSize,
	float& OutPosX, float& OutPosY)
{
	// Top-left
	float offsetX = -1.f * squareSize * GetCols();
	float offsetY = -1.f * squareSize * GetRows();

	OutPosX = offsetX + static_cast<float>(InPosX) * squareSize * 2;
	OutPosY = offsetY + static_cast<float>(InPosY) * squareSize * 2;
}

void Path::WorldPosToCoord(const float InPosX, const float InPosY, const size_t squareSize,
	unsigned int& OutPosX, unsigned int& OutPosY)
{
	float offsetX = -1.f * squareSize * GetCols();
	float offsetY = -1.f * squareSize * GetRows();

	OutPosX = static_cast<unsigned int>((InPosX - offsetX) / (squareSize * 2));
	OutPosY = static_cast<unsigned int>((InPosY - offsetY) / (squareSize * 2));
}

std::list<Node*> Path::GetConnections(unsigned int posX, unsigned int posY)
{
	// cout << "Get connections of: " << posX << " " << posY;
	std::list<Node*> connections;
	for (Node* elem : nodes)
	{
		// cout << "Elem x: " << elem.posX << " y: " << elem.posY << endl;

		// Left
		if (elem->posX == posX - 1 && elem->posY == posY)
			connections.push_back(elem);
		// Right
		else if (elem->posX == posX + 1 && elem->posY == posY)
			connections.push_back(elem);
		// Up
		else if (elem->posX == posX && elem->posY == posY - 1)
			connections.push_back(elem);
		// Down
		else if (elem->posX == posX && elem->posY == posY + 1)
			connections.push_back(elem);
	}

	// cout << endl;

	return connections;
}

void Path::BuildPath(Node* node)
{
	path.clear();

	cout << "Building path..." << endl;
	while (node->parent != nullptr)
	{
		path.push_back(node);
		node = node->parent;
	}
	cout << "Path built." << endl;
}

Node* Path::GetNodeAtPosition(unsigned int posX, unsigned int posY)
{
	for (Node* node : nodes)
	{
		if (node->posX == posX && node->posY == posY)
		{
			return node;
		}
	}

	return nullptr;
}
