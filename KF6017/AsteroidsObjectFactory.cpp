#include "AsteroidsObjectFactory.h"
#include "Spaceship.h"
#include "SpaceRock.h"
#include "Bullet.h"

#include "ObjectManager.h"
#include "ServiceManager.h"
#include "ErrorLogger.h"
#include "Explosion.h"


// PUBLIC

AsteroidsObjectFactory::AsteroidsObjectFactory() : Super()
{
}

AsteroidsObjectFactory::~AsteroidsObjectFactory()
{
}

std::weak_ptr<GameObject> AsteroidsObjectFactory::Create(ObjectType type, std::weak_ptr<ServiceManager> pServiceManager, Vector2D initPosition, float initAngle, float initScale)
{
	std::shared_ptr<ObjectManager> pObjectManager = pServiceManager.lock()->GetObjectManager().lock();
	if (!pObjectManager)
	{
		ErrorLogger::Writeln(L"AsteroidsObjectFactory::Create() - Failed to fetch object manager.");
		return std::weak_ptr<GameObject>();
	}

	std::shared_ptr<GameObject> pAsGameObject = nullptr;
	std::shared_ptr<ICollidableObject> pAsCollidableObject = nullptr;

	if (type == ObjectType::spaceship)
	{
		std::shared_ptr<Spaceship> pObj = std::make_shared<Spaceship>(pServiceManager);
		pAsGameObject = std::static_pointer_cast<GameObject>(pObj);
		pAsCollidableObject = std::static_pointer_cast<ICollidableObject>(pObj);
	} 
	else if (type == ObjectType::spacerock)
	{
		std::shared_ptr<SpaceRock> obj = std::make_shared<SpaceRock>(pServiceManager);
		pAsGameObject = std::static_pointer_cast<GameObject>(obj);
		pAsCollidableObject = std::static_pointer_cast<ICollidableObject>(obj);
	}
	else if (type == ObjectType::bullet)
	{
		std::shared_ptr<Bullet> obj = std::make_shared<Bullet>(pServiceManager);
		pAsGameObject = std::static_pointer_cast<GameObject>(obj);
		pAsCollidableObject = std::static_pointer_cast<ICollidableObject>(obj);
	}
	else if (type == ObjectType::explosion)
	{
		std::shared_ptr<Explosion> obj = std::make_shared<Explosion>(pServiceManager);
		pAsGameObject = std::static_pointer_cast<GameObject>(obj);
	}

	if (!pAsGameObject)
	{
		ErrorLogger::Writeln(L"AsteroidsObjectFactory::Create() - Failed to cast to GameObject.");
		return std::weak_ptr<GameObject>();
	}

	InitialiseGameObject(pAsGameObject, initPosition, initAngle, initScale);

	if(pAsCollidableObject)
		pObjectManager->AddCollidableObject(pAsGameObject, pAsCollidableObject);
	else
		pObjectManager->AddObject(pAsGameObject);

	return pAsGameObject;
}

void AsteroidsObjectFactory::InitialiseGameObject(std::shared_ptr<GameObject>& rpObj, Vector2D initPosition, float initAngle, float initScale)
{
	if (initPosition == GetDefaultPositionValue())
		rpObj->Initialise();
	else if (initAngle == GetDefaultAngleValue())
		rpObj->Initialise(initPosition);
	else if (initScale == GetDefaultScaleValue())
		rpObj->Initialise(initPosition, initAngle);
	else
		rpObj->Initialise(initPosition, initAngle, initScale);
}