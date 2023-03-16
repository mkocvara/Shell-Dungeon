#include "AsteroidsObjectFactory.h"
#include "Spaceship.h"
#include "SpaceRock.h"
#include "Bullet.h"

#include "ObjectManager.h"
#include "ServiceManager.h"
#include "ErrorLogger.h"


// PUBLIC

AsteroidsObjectFactory::AsteroidsObjectFactory() : Super()
{
}

AsteroidsObjectFactory::~AsteroidsObjectFactory()
{
}

std::weak_ptr<GameObject> AsteroidsObjectFactory::Create(ObjectType type, std::weak_ptr<ServiceManager> serviceManager, Vector2D initPosition, float initAngle, float initScale)
{
	std::shared_ptr<ObjectManager> pObjectManager = serviceManager.lock()->GetObjectManager().lock();
	if (!pObjectManager)
	{
		ErrorLogger::Writeln(L"AsteroidsObjectFactory::Create() - Failed to fetch object manager.");
		return std::weak_ptr<GameObject>();
	}

	std::shared_ptr<GameObject> asGameObject = nullptr;
	std::shared_ptr<ICollidableObject> asCollidableObject = nullptr;

	if (type == ObjectType::spaceship)
	{
		std::shared_ptr<Spaceship> obj = std::make_shared<Spaceship>(serviceManager);
		asGameObject = std::static_pointer_cast<GameObject>(obj);
		asCollidableObject = std::static_pointer_cast<ICollidableObject>(obj);
	} 
	else if (type == ObjectType::spacerock)
	{
		std::shared_ptr<SpaceRock> obj = std::make_shared<SpaceRock>(serviceManager);
		asGameObject = std::static_pointer_cast<GameObject>(obj);
		asCollidableObject = std::static_pointer_cast<ICollidableObject>(obj);
	}
	else if (type == ObjectType::bullet)
	{
		std::shared_ptr<Bullet> obj = std::make_shared<Bullet>(serviceManager);
		asGameObject = std::static_pointer_cast<GameObject>(obj);
		asCollidableObject = std::static_pointer_cast<ICollidableObject>(obj);
	}

	if (!asGameObject)
	{
		ErrorLogger::Writeln(L"AsteroidsObjectFactory::Create() - Failed to cast to GameObject.");
		return std::weak_ptr<GameObject>();
	}

	InitialiseGameObject(asGameObject, initPosition, initAngle, initScale);

	if(asCollidableObject)
		pObjectManager->AddCollidableObject(asGameObject, asCollidableObject);
	else
		pObjectManager->AddObject(asGameObject);

	return asGameObject;
}

void AsteroidsObjectFactory::InitialiseGameObject(std::shared_ptr<GameObject>& obj, Vector2D initPosition, float initAngle, float initScale)
{
	if (initPosition == GetDefaultPositionValue())
		obj->Initialise();
	else if (initAngle == GetDefaultAngleValue())
		obj->Initialise(initPosition);
	else if (initScale == GetDefaultScaleValue())
		obj->Initialise(initPosition, initAngle);
	else
		obj->Initialise(initPosition, initAngle, initScale);
}