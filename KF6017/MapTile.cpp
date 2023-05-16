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