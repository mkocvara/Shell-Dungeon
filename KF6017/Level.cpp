#include "Level.h"
#include <fstream>
#include <string>
#include <utility>
#include "StringUtil.h"
#include "ErrorLogger.h"
#include <algorithm>

#include "ServiceManager.h"
#include "GameObjectFactory.h"
#include "mydrawengine.h"
#include "GameObject.h"

#define NOMINMAX
#undef max
#undef min


namespace {
	struct TileInfo {
		const char symbol;
		const std::wstring path;
	};

	const TileInfo tileEmpty = { '.', L""};
	const TileInfo tileWall = { '#', L"Assets/Environment/Walls/wall" };
	const TileInfo tileFloor = { '_', L"Assets/Environment/Floor/floor" };

	const std::wstring fileTopAdd = L"_top";
	const std::wstring fileBottomAdd = L"_bottom";
	const std::wstring fileLeftAdd = L"_left";
	const std::wstring fileRightAdd = L"_right";

	const std::wstring fileInnerWallAdd = L"_inner";
	const std::wstring fileOuterWallAdd = L"_outer";
	const std::wstring fileCombinedWallAdd = L"_combined";

	const std::wstring tileFileExtension = L".png";

	struct EntityInfo {
		const char symbol;
		const ObjectType objectType;
	};

	const EntityInfo knightEntity = { 'K', ObjectType::knight };
	const EntityInfo orcEntity = { 'O', ObjectType::orc };
};


// PUBLIC

Level::Level(const std::weak_ptr<ServiceManager> pServiceManager, const int levelNumber, const std::shared_ptr<GameObject> pKnight)
	: mpServiceManager(pServiceManager)
{
	const std::shared_ptr<ServiceManager> pServiceManagerLocked = mpServiceManager.lock();

	std::ifstream levelFile;
	const std::string filePath = "Levels/" + std::to_string(levelNumber);
	levelFile.open(filePath);

	std::string line;
	const std::wstring errorMessage = (L"DungeonGameManager(); Failed to load level " + std::to_wstring(levelNumber) + L", file not formatted correctly.");


	while (std::getline(levelFile, line))
	{
		if (levelFile.fail() || levelFile.eof())
		{
			ErrorLogger::Writeln(errorMessage.c_str());
			return;
		}

		StringUtil::trim(line);

		if (!(line[0] == '#' || line.empty())) // allow comments at the start of level files
			break;
	}

	if (line != "MAP")
	{
		ErrorLogger::Writeln(errorMessage.c_str());
		return;
	}

	const int MAX_LINES = 1000; // Limit lines to avoid infinite loop if level file is not formatted correctly.
	int lines = 0;
	while (std::getline(levelFile, line))
	{
		if (levelFile.fail() || levelFile.eof() || ++lines > MAX_LINES)
		{
			ErrorLogger::Writeln(errorMessage.c_str());
			return;
		}
		else if (StringUtil::trim_copy(line) == "ENTITIES")
		{
			break;
		}
		else
		{
			mMapSymbols.push_back(line);
		}
	}

	const int numRows = (int)mMapSymbols.size();
	const int numCols = (int)mMapSymbols[0].size();

	// Check that each line is the same length
	for (int i = 1; i < numRows; i++)
	{
		if (mMapSymbols[i].size() != numCols)
		{
			ErrorLogger::Writeln(errorMessage.c_str());
			ErrorLogger::Writeln(L"> (Level must be formatted as a rectangle; each line must have the same length.)");
			return;
		}
	}

	const std::shared_ptr<GameObjectFactory> pObjectFactory = pServiceManagerLocked->GetObjectFactory().lock();
	for (int i = 0; i < numRows; i++)
	{
		const std::string row = mMapSymbols[i];
		std::vector<std::weak_ptr<GameObject>> tilesRow;

		for (int j = 0; j < numCols; j++)
		{
			std::shared_ptr<GameObject> pTile;
			PictureIndex tileSprite;
			std::wstring tilePath = tileWall.path;

			if (row[j] == tileEmpty.symbol)
			{
				tilesRow.push_back(pTile);
				continue;
			}
			else if (row[j] == tileWall.symbol)
			{	
				const AdjacencyFlags adjacentFloors = CheckSymbolAdjacent(i, j, tileFloor.symbol);
				const AdjacencyFlags adjacentWalls = CheckSymbolAdjacent(i, j, tileWall.symbol);

				short numFloorsCardinal = 0;

				if (adjacentFloors.bottom)
				{
					tilePath += fileTopAdd;
					numFloorsCardinal++;
				}
				if (adjacentFloors.top)
				{
					tilePath += fileBottomAdd;
					numFloorsCardinal++;
				}
				if (adjacentFloors.right)
				{
					tilePath += fileLeftAdd;
					numFloorsCardinal++;
				}
				if (adjacentFloors.left)
				{
					tilePath += fileRightAdd;
					numFloorsCardinal++;
				}

				// outer corners (no adjacent floors)
				if (numFloorsCardinal == 0)
				{
					bool outerTop, outerBottom, outerLeft, outerRight;
					outerTop = outerBottom = outerLeft = outerRight = false;

					if (adjacentFloors.topLeft)
						outerBottom = outerRight = true;
					if (adjacentFloors.topRight)
						outerBottom = outerLeft = true;
					if (adjacentFloors.bottomLeft)
						outerTop = outerRight = true;
					if (adjacentFloors.bottomRight)
						outerTop = outerLeft = true;

					// a wall entirely surrounded by other walls or empty tiles
					if (!(outerTop || outerBottom || outerLeft || outerRight))
					{
						continue; // do not create a tile
					}

					tilePath += (outerTop ? fileTopAdd : L"")
						+ (outerBottom ? fileBottomAdd : L"")
						+ (outerLeft ? fileLeftAdd : L"")
						+ (outerRight ? fileRightAdd : L"");

					tilePath += fileOuterWallAdd;
				}

				// inner corners (multiple adjacent floor tiles)
				/*	note:
				*	___
				*	###  <-- this is not a corner
				*	___
				*/	
				if (numFloorsCardinal > 1 
					&& (!(numFloorsCardinal == 2 && ((adjacentFloors.bottom && adjacentFloors.top) || (adjacentFloors.left && adjacentFloors.right)))))
				{
					 
					tilePath += fileInnerWallAdd;

					// turn / inner-outer corner
					if (numFloorsCardinal == 2
						&& adjacentFloors.topLeft
						&& adjacentFloors.topRight
						&& adjacentFloors.bottomLeft
						&& adjacentFloors.bottomRight)
						{
							tilePath += fileOuterWallAdd;
						}
				}

				// Corner+straight combined tiles
				/* e.g.: (see middle one)
				*  1)	_#_		2)	___
				*		##_			###
				*		.##			.#_
				*/
				if (numFloorsCardinal == 1)
				{
					if ((adjacentFloors.top && (adjacentFloors.bottomLeft || adjacentFloors.bottomRight)) ||
						(adjacentFloors.bottom && (adjacentFloors.topLeft || adjacentFloors.topRight)) ||
						(adjacentFloors.left && (adjacentFloors.topRight || adjacentFloors.bottomRight)) ||
						(adjacentFloors.right && (adjacentFloors.topLeft || adjacentFloors.bottomLeft))
						)
					{
						tilePath += fileCombinedWallAdd;

						bool cornerTop, cornerBottom, cornerLeft, cornerRight;
						cornerTop = cornerBottom = cornerLeft = cornerRight = false;

						if (adjacentFloors.topLeft && !(adjacentFloors.top || adjacentFloors.left))
							cornerBottom = cornerRight = true;
						if (adjacentFloors.topRight && !(adjacentFloors.top || adjacentFloors.right))
							cornerBottom = cornerLeft = true;
						if (adjacentFloors.bottomLeft && !(adjacentFloors.bottom || adjacentFloors.left))
							cornerTop = cornerRight = true;
						if (adjacentFloors.bottomRight && !(adjacentFloors.bottom || adjacentFloors.right))
							cornerTop = cornerLeft = true;

						tilePath += (cornerTop ? fileTopAdd : L"")
							+ (cornerBottom ? fileBottomAdd : L"")
							+ (cornerLeft ? fileLeftAdd : L"")
							+ (cornerRight ? fileRightAdd : L"");
					}
				}

				tilePath += tileFileExtension;

				pTile = pObjectFactory->Create(ObjectType::mapTileCollidable, false).lock();
			}
			else if (row[j] == tileFloor.symbol)
			{
				pTile = pObjectFactory->Create(ObjectType::mapTile, false).lock();

				tilePath = tileFloor.path;

				const AdjacencyFlags adjacentWalls = CheckSymbolAdjacent(i, j, tileWall.symbol);

				// TODO: Could add inner corners but it is low priority
				if (adjacentWalls.top)
					tilePath += fileTopAdd;
				if (adjacentWalls.bottom)
					tilePath += fileBottomAdd;
				if (adjacentWalls.left)
					tilePath += fileLeftAdd;
				if (adjacentWalls.right)
					tilePath += fileRightAdd;

				tilePath += tileFileExtension;
			}
			else
			{
				ErrorLogger::Writeln(L"Level(); Unrecognised symbol in level file " + levelNumber);
				tilesRow.push_back(pTile);
				continue;
			}

			const std::shared_ptr<MyDrawEngine> pDrawEngine = pServiceManagerLocked->GetDrawEngine().lock();
			tileSprite = pDrawEngine->LoadPicture(tilePath.c_str());

			int tileHeight, tileWidth;
			pDrawEngine->GetDimensions(tileSprite, tileHeight, tileWidth);
			const float tileScale = 1.f / ((float)tileHeight / (float)mTileSize);

			const float mapWidth = (float)mTileSize * numCols;
			const float mapHeight = (float)mTileSize * numRows;

			pTile->Initialise(Vector2D((float)(mTileSize * j) - mapWidth/2, (float)(-mTileSize * i) + mapHeight / 2), 0.f, tileScale);
			pTile->SetRenderSprite(tileSprite);
			tilesRow.push_back(pTile);
		}

		mMapTiles.push_back(tilesRow);
	}


	const float mapWidth = (float)mTileSize * numCols;
	const float mapHeight = (float)mTileSize * numRows;
	const std::wstring errorMessageEntities = (L"DungeonGameManager(); Error while loading entities for level " + std::to_wstring(levelNumber) + L", file not formatted correctly.");
	for (int i = 0; i < numRows; i++)
	{
		std::getline(levelFile, line);
		if (levelFile.fail() || levelFile.eof())
		{
			ErrorLogger::Writeln(errorMessageEntities.c_str());
			return;
		}

		int lineSize = std::min((int)line.size(), numCols);
		for (int j = 0; j < lineSize; j++)
		{
			if (line[j] == knightEntity.symbol)
			{
				Vector2D startPos = Vector2D((float)(mTileSize * j) - mapWidth / 2, (float)(-mTileSize * i) + mapHeight / 2);
				pKnight->SetPosition(startPos);
			}
			else if (line[j] == orcEntity.symbol)
			{
				Vector2D startPos = Vector2D((float)(mTileSize * j) - mapWidth / 2, (float)(-mTileSize * i) + mapHeight / 2);
				pObjectFactory->Create(orcEntity.objectType, true, startPos);
			}
		}
	}

	levelFile.close();
}

Level::~Level()
{
	for (std::vector<std::weak_ptr<GameObject>> row : mMapTiles)
	{
		for (std::weak_ptr<GameObject> pTile : row)
		{
			std::shared_ptr<GameObject> pTileLocked = pTile.lock();
			if (pTileLocked)
				pTileLocked->Remove();
		}
	}
}


// PROTECTED

Level::AdjacencyFlags Level::CheckSymbolAdjacent(int row, int col, char symbol)
{
	AdjacencyFlags flags;

	const int maxRow = (int)mMapSymbols.size() - 1;
	const int maxCol = (int)mMapSymbols[row].size() - 1;

	flags.top = (row > 0 && mMapSymbols[row - 1][col] == symbol);
	flags.bottom = (row < maxRow && mMapSymbols[row + 1][col] == symbol);
	flags.left = (col > 0 && mMapSymbols[row][col - 1] == symbol);
	flags.right = (col < maxCol&& mMapSymbols[row][col + 1] == symbol);

	flags.topLeft = (row > 0 && col > 0 && mMapSymbols[row - 1][col - 1] == symbol);
	flags.topRight = (row > 0 && col < maxCol && mMapSymbols[row - 1][col + 1] == symbol);
	flags.bottomLeft = (row < maxRow && col > 0 && mMapSymbols[row + 1][col - 1] == symbol);
	flags.bottomRight = (col < maxCol && row < maxRow && mMapSymbols[row + 1][col + 1] == symbol);

	return flags;
}
