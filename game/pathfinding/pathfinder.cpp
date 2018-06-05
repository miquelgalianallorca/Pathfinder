#include <stdafx.h>

#include "pathfinder.h"
#include "path.h"

Pathfinder::Pathfinder() : MOAIEntity2D()
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
	// A*
	path.AStar(GetStartPosition().mX, GetStartPosition().mY, GetEndPosition().mX, GetEndPosition().mY);
	
}

void Pathfinder::DrawDebug()
{
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();
	gfxDevice.SetPenColor(1.f, 1.f, 1.f, 1.f);
	gfxDevice.SetPenWidth(1.f);

	// Draw grid
	USRect rect;
	rect.mXMin = -1.f * squareSize * path.GetCols();
	rect.mXMax =        squareSize * path.GetCols();
	rect.mYMin = -1.f * squareSize * path.GetRows();
	rect.mYMax =        squareSize * path.GetRows();
	gfxDevice.SetPenColor(0.f, .3f, 0.f, 1.f);
	
	// Draw obstacles
	path.DrawDebug(squareSize);

	gfxDevice.SetPenColor(1.f, 1.f, 1.f, 1.f);
	MOAIDraw::DrawGrid(rect, 10, 5);
}

// Returns true if pathfinding process finished
bool Pathfinder::PathfindStep()
{
	// Optional P6: do it step by step
	// ...
    return true;
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

	float pX = state.GetValue<float>(2, 0.0f);
	float pY = state.GetValue<float>(3, 0.0f);
	self->SetStartPosition(pX, pY);
	return 0;
}

int Pathfinder::_setEndPosition(lua_State* L)
{
	MOAI_LUA_SETUP(Pathfinder, "U")

	float pX = state.GetValue<float>(2, 0.0f);
	float pY = state.GetValue<float>(3, 0.0f);
	self->SetEndPosition(pX, pY);
	return 0;
}

int Pathfinder::_pathfindStep(lua_State* L)
{
    MOAI_LUA_SETUP(Pathfinder, "U")

    self->PathfindStep();
    return 0;
}