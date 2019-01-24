#include <stdafx.h>

#include "pathfinder.h"
#include "path.h"

using std::cout;
using std::endl;

Pathfinder::Pathfinder() :
	MOAIEntity2D(),
	IsStartPositionSet(false),
	IsEndPositionSet(false)
{
	RTTI_BEGIN
		RTTI_EXTEND(MOAIEntity2D)
	RTTI_END

	path.Load("../sample/path.txt", 5, 10);
	squareSize = 40;
}

Pathfinder::~Pathfinder()
{
}

// Calculates path
void Pathfinder::UpdatePath()
{
	// Check if valid path
	if (!IsStartPositionSet || !IsEndPositionSet)
	{
		cout << "Path doesn't have start and end." << endl;
		return;
	}

	// Mouse position to grid coords
	unsigned int posX, posY;
	path.WorldPosToCoord(m_StartPosition.mX, m_StartPosition.mY, squareSize, posX, posY);
	m_StartPositionCoord = USVec2D(static_cast<float>(posX), static_cast<float>(posY));
	path.WorldPosToCoord(m_EndPosition.mX, m_EndPosition.mY, squareSize, posX, posY);
	m_EndPositionCoord = USVec2D(static_cast<float>(posX), static_cast<float>(posY));

	if (!path.IsValidCoord(m_StartPositionCoord) || !path.IsValidCoord(m_EndPositionCoord))
	{
		cout << "Invalid coords." << endl;
		return;
	}

	// A*
	path.AStar(m_StartPositionCoord.mX, m_StartPositionCoord.mY,
		m_EndPositionCoord.mX, m_EndPositionCoord.mY);
}

void Pathfinder::DrawDebug()
{
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();
	gfxDevice.SetPenColor(1.f, 1.f, 1.f, 1.f);
	gfxDevice.SetPenWidth(1.f);

	// Draw grid
	USRect rect;
	rect.mXMin = -1.f * squareSize * path.GetCols();
	rect.mXMax = static_cast<float>(squareSize * path.GetCols());
	rect.mYMin = -1.f * squareSize * path.GetRows();
	rect.mYMax = static_cast<float>(squareSize * path.GetRows());
	
	// Draw start pos
	gfxDevice.SetPenColor(0.f, 1.f, 0.f, 1.f);
	float posX, posY;
	path.CoordToWorldPos(static_cast<unsigned int>(m_StartPositionCoord.mX), static_cast<unsigned int>(m_StartPositionCoord.mY),
		squareSize, posX, posY);
	MOAIDraw::DrawRectFill(posX, posY, posX + squareSize * 2, posY + squareSize * 2);
	// Draw end pos
	gfxDevice.SetPenColor(0.f, 0.f, 1.f, 1.f);
	path.CoordToWorldPos(static_cast<unsigned int>(m_EndPositionCoord.mX), static_cast<unsigned int>(m_EndPositionCoord.mY),
		squareSize, posX, posY);
	MOAIDraw::DrawRectFill(posX, posY, posX + squareSize * 2, posY + squareSize * 2);

	// Draw obstacles
	path.DrawDebug(squareSize);

	gfxDevice.SetPenColor(1.f, 1.f, 1.f, 1.f);
	MOAIDraw::DrawGrid(rect, 10, 5);
}

// Returns true if pathfinding process finished
bool Pathfinder::PathfindStep()
{
	// Single iteration of A* loop
	path.AStarStep();

	return true;
}

void Pathfinder::SetStartPosition(float x, float y)
{
	m_StartPosition = USVec2D(x, y);
	IsStartPositionSet = true;
	UpdatePath();
}

void Pathfinder::SetEndPosition(float x, float y)
{
	m_EndPosition = USVec2D(x, y);
	IsEndPositionSet = true;
	UpdatePath();
}


//lua configuration ----------------------------------------------------------------//
void Pathfinder::RegisterLuaFuncs(MOAILuaState& state)
{
	MOAIEntity::RegisterLuaFuncs(state);

	luaL_Reg regTable [] = {
		{ "setStartPosition",		_setStartPosition},
		{ "setEndPosition",			_setEndPosition},
        { "pathfindStep",           _pathfindStep},
		{ NULL, NULL }
	};

	luaL_register(state, 0, regTable);
}

int Pathfinder::_setStartPosition(lua_State* L)
{
	MOAI_LUA_SETUP(Pathfinder, "U")

	const float pX = state.GetValue<float>(2, 0.0f);
	const float pY = state.GetValue<float>(3, 0.0f);
	self->SetStartPosition(pX, pY);
	return 0;
}

int Pathfinder::_setEndPosition(lua_State* L)
{
	MOAI_LUA_SETUP(Pathfinder, "U")

	const float pX = state.GetValue<float>(2, 0.0f);
	const float pY = state.GetValue<float>(3, 0.0f);
	self->SetEndPosition(pX, pY);
	return 0;
}

int Pathfinder::_pathfindStep(lua_State* L)
{
    MOAI_LUA_SETUP(Pathfinder, "U")

    self->PathfindStep();
    return 0;
}