#ifndef __PATHFINDER_H__
#define __PATHFINDER_H__

#include <moaicore/MOAIEntity2D.h>
#include "path.h"

class Pathfinder: public virtual MOAIEntity2D
{
public:
	Pathfinder();
	~Pathfinder();

	virtual void DrawDebug();

	void SetStartPosition(float x, float y);
	void SetEndPosition(float x, float y);
	const USVec2D& GetStartPosition() const { return m_StartPosition; }
	const USVec2D& GetEndPosition()   const { return m_EndPosition;   }

    bool PathfindStep();

private:
	void UpdatePath();

	USVec2D m_StartPosition;
	USVec2D m_EndPosition;
	
	// My vars ===========================================
	Path   path;       // Map and nodes
	size_t squareSize; // Map coords to pixel coords
	
	USVec2D m_StartPositionCoord;
	USVec2D m_EndPositionCoord;

	bool IsStartPositionSet;
	bool IsEndPositionSet;
	// ===================================================

	// Lua configuration
public:
	DECL_LUA_FACTORY(Pathfinder)
public:
	virtual void RegisterLuaFuncs(MOAILuaState& state);
private:
	static int _setStartPosition(lua_State* L);
	static int _setEndPosition(lua_State* L);
    static int _pathfindStep(lua_State* L);
};


#endif