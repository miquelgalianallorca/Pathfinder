#pragma once

struct Node
{
	Node() {}
	Node(unsigned int _posX, unsigned int _posY, unsigned int _cost) :
		posX(_posX),
		posY(_posY),
		cost(_cost),
		parent(nullptr)
	{}

	friend bool operator==(const Node& lhs, const Node& rhs)
	{ 
		if (lhs.posX == rhs.posX && lhs.posY == rhs.posY)
			return true;
		else return false;
	}

	unsigned int posX;
	unsigned int posY;   // Map grid location
	int   cost;          // Traverse cost of grid location
	Node* parent;
	
	float g;  // Cost from origin to pos
	float f;  // f = g + h, h = Estimated cost from pos to goal
};

class Path
{
public:
	Path() :
		rows(0),
		cols(0)
	{}
	
	void Load(const std::string& filename, const unsigned int rows, const unsigned int cols);
	bool AStar(const float startX, const float startY, const float endX, const float endY);
	void BuildPath(Node* node);

	std::string  GetMap()  const { return mapSrc; }
	unsigned int GetRows() const { return rows;   }
	unsigned int GetCols() const { return cols;   }

	void DrawDebug(const size_t& squareSize);

	void CoordToWorldPos(const unsigned int InPosX, const unsigned int InPosY, const size_t squareSize,
		float& OutPosX, float& OutPosY);
	void WorldPosToCoord(const float InPosX, const float InPosY, const size_t squareSize, 
		unsigned int& OutPosX, unsigned int& OutPosY);

private:
	unsigned int rows, cols;
	std::string mapSrc;
	std::vector<Node*> nodes;
	
	static bool OrderByShortest(const Node& first, const Node& second)
	{
		return (first.cost < second.cost);
	}
	std::list<Node*> GetConnections(unsigned int posX, unsigned int posY);
};