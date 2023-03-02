#include "ObjectManager.h"
#include "GameObject.h"
#include <winerror.h>


// PUBLIC

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
	mGameObjects.clear();
}

void ObjectManager::AddObject(GameObject* newObject)
{
	std::unique_ptr<GameObject> pObj(newObject);
	AddObject(pObj);
}

void ObjectManager::AddObject(std::unique_ptr<GameObject>& newObject)
{
	mGameObjects.push_back(std::move(newObject));
}

ErrorType ObjectManager::UpdateAll(double deltaTime)
{
	for (const std::unique_ptr<GameObject>& gameObject : mGameObjects)
	{
		if (!gameObject)
			continue;

		if (FAILED(gameObject->Update(deltaTime)))
			return FAILURE;
	}

	return SUCCESS;
}
