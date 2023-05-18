/*
* MapTile.h
* 19002175
*
* A static map tile with no collisions (e.g., floor, ground).
*/

#pragma once
#include "GameObject.h"

class MapTile : public GameObject
{
	typedef GameObject Super;

public:
	MapTile(std::weak_ptr<ServiceManager> pServiceManager);
	virtual ~MapTile();
};

