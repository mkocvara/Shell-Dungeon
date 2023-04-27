#include "ErrorLogger.h"

#include "ServiceManager.h"
#include "ObjectManager.h"
#include "DungeonObjectFactory.h"

#include "Knight.h"


// PUBLIC

DungeonObjectFactory::DungeonObjectFactory() : Super()
{
}

DungeonObjectFactory::~DungeonObjectFactory()
{
}

std::weak_ptr<GameObject> DungeonObjectFactory::Create(ObjectType type, std::weak_ptr<ServiceManager> pServiceManager, Vector2D initPosition, float initAngle, float initScale)
{
	std::shared_ptr<ObjectManager> pObjectManager = pServiceManager.lock()->GetObjectManager().lock();
	if (!pObjectManager)
	{
		ErrorLogger::Writeln(L"DungeonObjectFactory::Create() - Failed to fetch object manager.");
		return std::weak_ptr<GameObject>();
	}

	std::shared_ptr<GameObject> pAsGameObject = nullptr;
	std::shared_ptr<ICollidableObject> pAsCollidableObject = nullptr;

	if (type == ObjectType::knight)
	{
		std::shared_ptr<Knight> pObj = std::make_shared<Knight>(pServiceManager);
		pAsGameObject = std::static_pointer_cast<GameObject>(pObj);
		pAsCollidableObject = std::static_pointer_cast<ICollidableObject>(pObj);
	} 
	/*else if (type == ObjectType::spacerock)
	{
		std::shared_ptr<SpaceRock> obj = std::make_shared<SpaceRock>(pServiceManager);
		pAsGameObject = std::static_pointer_cast<GameObject>(obj);
		pAsCollidableObject = std::static_pointer_cast<ICollidableObject>(obj);
	}*/

	if (!pAsGameObject)
	{
		ErrorLogger::Writeln(L"DungeonObjectFactory::Create() - Failed to cast to GameObject.");
		return std::weak_ptr<GameObject>();
	}

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