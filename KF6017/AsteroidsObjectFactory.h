#pragma once
#include "GameObjectFactory.h"

class AsteroidsObjectFactory : public GameObjectFactory
{
public:
	AsteroidsObjectFactory();
	virtual ~AsteroidsObjectFactory();

	typedef GameObjectFactory Super;

	virtual std::unique_ptr<GameObject> Create(ObjectType type, std::weak_ptr<ServiceManager> serviceManager) override;
};

