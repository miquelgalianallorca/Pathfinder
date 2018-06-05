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
		node.posY = i / cols;
		node.posX = i % cols;
		nodes.push_back(node);
	}
}

// Heuristics: Euclidean distance. Manhattan could give longer paths than necessary.
bool Path::AStar(const float startX, const float startY, const float endX, const float endY)
{


	return true;
}

void Path::DrawDebug(const size_t& squareSize)
{
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();
	
	for (auto node : nodes)
	{
		if (node.cost > 0)
		{
			float alpha = (node.cost) / 10.f;
			gfxDevice.SetPenColor(1.f, 0.f, 0.f, .1f);

			float wPosX, wPosY;
			CoordToWorldPos(node.posX, node.posY, squareSize, wPosX, wPosY);
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