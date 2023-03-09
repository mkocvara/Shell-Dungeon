#pragma once
#include <memory>
#include "objecttypes.h"

class GameObject;
class ServiceManager;

class GameObjectFactory
{
public:
	GameObjectFactory();
	~GameObjectFactory();

	virtual std::unique_ptr<GameObject> Create(ObjectType type, std::weak_ptr<ServiceManager> serviceManager) =0;
};