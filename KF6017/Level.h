/*
* Level.h
* 19002175
*
* Represents a level. Is constructed with level number. Loads a level file, parses it, and constructs the map
* based on its formatting, algorithmically determining what type of sprite to use for each tile in order to
* achieve continuous walls and floors. Also places the Knight and creates enemies based on the same file.
*/

#pragma once
#include "errortype.h"
#include <vector>
#include <memory>
#include <string>

class ServiceManager;
class GameObject;

class Level
{
	typedef int PictureIndex;
	
public:
	// Takes the Knight as a parameter in order to initialise its position based on the level layout; does not keep a reference.
	Level(const std::weak_ptr<ServiceManager> pServiceManager, const int levelNumber, const std::shared_ptr<GameObject> pKnight);
	~Level();

	int GetTotalEnemies() const;

protected:
	struct AdjacencyFlags {
		bool top, bottom, left, right, topLeft, topRight, bottomLeft, bottomRight;
	};

	AdjacencyFlags CheckSymbolAdjacent(int row, int col, char symbol);

private:
	std::vector<std::string> mMapSymbols;
	std::vector<std::vector<std::weak_ptr<GameObject>>> mMapTiles;
	std::weak_ptr<ServiceManager> mpServiceManager;

	const int mTileSize = 128;

	int mNumEnemies = 0;
};

