#include "AsteroidsObjectFactory.h"
#include "Spaceship.h"
#include "SpaceRock.h"
#include "Bullet.h"


// PUBLIC

AsteroidsObjectFactory::AsteroidsObjectFactory() : Super()
{
}

AsteroidsObjectFactory::~AsteroidsObjectFactory()
{
}

std::unique_ptr<GameObject> AsteroidsObjectFactory::Create(ObjectType type, std::weak_ptr<ServiceManager> serviceManager)
{
	//std::unique_ptr<GameObject> obj;

	switch (type)
	{
	case ObjectType::spaceship:
		return std::make_unique<Spaceship>(serviceManager);
	case ObjectType::spacerock:
		return std::make_unique<SpaceRock>(serviceManager);
	case ObjectType::bullet:
		return std::make_unique<Bullet>(serviceManager);
	default:
		return nullptr;
	}

	/*obj->Initialise();
	return obj;*/
}