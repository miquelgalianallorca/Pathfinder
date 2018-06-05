#pragma once


struct Node
{
	unsigned int posX, posY;
	int cost;
	
	int g;
	int f;
};

class Path
{
public:
	Path() :
		rows(0),
		cols(0)
	{}
	
	void Load(const std::string& filename, const unsigned int rows, const unsigned int cols);

	std::string GetMap() const { return mapSrc; }

private:
	unsigned int rows, cols;
	std::string mapSrc;
	std::vector<Node> nodes;
};