#pragma once
#include "GameObject.h"

class MapTile : public GameObject
{
	typedef GameObject Super;

public:
	MapTile(std::weak_ptr<ServiceManager> pServiceManager);
	virtual ~MapTile();
};

