#include <stdafx.h>
#include <algorithm>
#include "path.h"

void Path::Load(const std::string& filename, const unsigned int rows, const unsigned int cols)
{
	this->rows = rows;
	this->cols = cols;

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
		Node node;
		node.cost = static_cast<int>(mapSrc.at(i)) - static_cast<int>('0');
		node.posX = i / cols;
		node.posY = i % cols;
		nodes.push_back(node);
	}
}

