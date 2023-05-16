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
};

