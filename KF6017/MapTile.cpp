/*
* MapTile.cpp
* 19002175
*
* A static map tile with no collisions (e.g., floor, ground).
*/

#include "MapTile.h"
#include "RenderLayers.h"


// PUBLIC

MapTile::MapTile(std::weak_ptr<ServiceManager> pServiceManager)
	: Super(pServiceManager)
{
	mObjectType = ObjectType::mapTile;
	SetZIndex(RenderLayer::MapLayer);
}

MapTile::~MapTile()
{
}