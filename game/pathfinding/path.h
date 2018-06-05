#pragma once

struct Node
{
	unsigned int posX;
	unsigned int posY;   // Map grid location
	int cost;            // Traverse cost of grid location
	
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
	
	std::string  GetMap()  const { return mapSrc; }
	unsigned int GetRows() const { return rows;   }
	unsigned int GetCols() const { return cols;   }

	void DrawDebug(const size_t& squareSize);

private:
	void CoordToWorldPos(const unsigned int InPosX, const unsigned int InPosY, const size_t squareSize,
		float& OutPosX, float& OutPosY);

	unsigned int rows, cols;
	std::string mapSrc;
	std::vector<Node> nodes;
};