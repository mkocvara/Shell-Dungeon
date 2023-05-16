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
	Level(const std::weak_ptr<ServiceManager> pServiceManager, int levelNumber);
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

