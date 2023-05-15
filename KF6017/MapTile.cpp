#include "MapTile.h"


// PUBLIC

MapTile::MapTile(std::weak_ptr<ServiceManager> pServiceManager)
	: Super(pServiceManager)
{
	mObjectType = ObjectType::mapTile;
}

MapTile::~MapTile()
{
}