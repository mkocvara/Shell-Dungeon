#include "ErrorLogger.h"

#include "ServiceManager.h"
#include "ObjectManager.h"
#include "DungeonObjectFactory.h"

#include "Knight.h"
#include "Orc.h"
#include "Attack.h"


// PUBLIC

DungeonObjectFactory::DungeonObjectFactory(std::weak_ptr<ServiceManager> pServiceManager)
	: Super(pServiceManager)
{
}

DungeonObjectFactory::~DungeonObjectFactory()
{
}

std::weak_ptr<GameObject> DungeonObjectFactory::Create(ObjectType type, bool initialise, Vector2D initPosition, float initAngle, float initScale)
{
	std::shared_ptr<ObjectManager> pObjectManager = mpServiceManager.lock()->GetObjectManager().lock();
	if (!pObjectManager)
	{
		ErrorLogger::Writeln(L"DungeonObjectFactory::Create() - Failed to fetch object manager.");
		return std::weak_ptr<GameObject>();
	}

	std::shared_ptr<GameObject> pAsGameObject = nullptr;
	std::shared_ptr<ICollidableObject> pAsCollidableObject = nullptr;

	if (type == ObjectType::knight)
	{
		std::shared_ptr<Knight> pObj = std::make_shared<Knight>(mpServiceManager);
		pAsGameObject = std::static_pointer_cast<GameObject>(pObj);
		pAsCollidableObject = std::static_pointer_cast<ICollidableObject>(pObj);
	} 
	else if (type == ObjectType::orc)
	{
		std::shared_ptr<Orc> obj = std::make_shared<Orc>(mpServiceManager);
		pAsGameObject = std::static_pointer_cast<GameObject>(obj);
		pAsCollidableObject = std::static_pointer_cast<ICollidableObject>(obj);
	}
	else if (type == ObjectType::attack)
	{
		std::shared_ptr<Attack> obj = std::make_shared<Attack>(mpServiceManager);
		pAsGameObject = std::static_pointer_cast<GameObject>(obj);
		pAsCollidableObject = std::static_pointer_cast<ICollidableObject>(obj);
	}

	if (!pAsGameObject)
	{
		ErrorLogger::Writeln(L"DungeonObjectFactory::Create() - Failed to cast to GameObject.");
		return std::weak_ptr<GameObject>();
	}

	if (initialise)
		InitialiseGameObject(pAsGameObject, initPosition, initAngle, initScale);

	if(pAsCollidableObject)
		pObjectManager->AddCollidableObject(pAsGameObject, pAsCollidableObject);
	else
		pObjectManager->AddObject(pAsGameObject);

	return pAsGameObject;
}

void DungeonObjectFactory::InitialiseGameObject(std::shared_ptr<GameObject>& rpObj, Vector2D initPosition, float initAngle, float initScale)
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