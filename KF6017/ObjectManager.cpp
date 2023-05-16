#include "ObjectManager.h"
#include "GameObject.h"
#include "ICollidableObject.h"
#include <winerror.h>
#include <algorithm>

#include "ServiceManager.h"
#include "MyDrawEngine.h"

// PUBLIC

ObjectManager::ObjectManager()
	: Super()
{
}

ObjectManager::~ObjectManager()
{
	mGameObjectsList.clear();
}

void ObjectManager::AddObject(std::shared_ptr<GameObject>& rpNewObject)
{
	mGameObjectsList.push_back(rpNewObject);
	mObjectsListIsSorted = false;
}

void ObjectManager::AddCollidableObject(std::shared_ptr<GameObject>& rpAsGameObject, std::shared_ptr<ICollidableObject>& rpAsCollidableObject)
{
	AddObject(rpAsGameObject);
	mObjectsCollidableMap.emplace(rpAsGameObject, rpAsCollidableObject);
}

ErrorType ObjectManager::UpdateAll(double deltaTime)
{
	if (!mObjectsListIsSorted)
	{
		// sorts by zIndex in ascending order
		mGameObjectsList.sort([](const std::shared_ptr<GameObject>& a, const std::shared_ptr<GameObject>& b) {return a->GetZIndex() < b->GetZIndex(); });

		mObjectsListIsSorted = true;
	}

	for (const std::shared_ptr<GameObject>& rpGameObject : mGameObjectsList)
	{
		if (!rpGameObject)
			continue;

		if (FAILED(rpGameObject->Update(deltaTime)))
			return FAILURE;
	}

	CheckCollisions();

	// only run object removal every 5 seconds
	static double removeDelay = 0.0;
	removeDelay += deltaTime;
	if (removeDelay > 5.0) 
	{
		RemoveDeletedObjects();
		removeDelay -= 5.0;
	}

	return SUCCESS;
}

void ObjectManager::Clear()
{
	mGameObjectsList.clear();
	mObjectsCollidableMap.clear();
}

std::list<std::shared_ptr<GameObject>> ObjectManager::GetAllObjects() const
{
	return mGameObjectsList;
}

int ObjectManager::GetNumberOfObjects() const
{
	return mGameObjectsList.size();
}

void ObjectManager::CheckCollisions()
{
	std::map<std::shared_ptr<GameObject>, std::shared_ptr<ICollidableObject>>::iterator it1;
	std::map<std::shared_ptr<GameObject>, std::shared_ptr<ICollidableObject>>::iterator it2;
	for (it1 = mObjectsCollidableMap.begin(); it1 != mObjectsCollidableMap.end(); ++it1)
	{
		for (it2 = std::next(it1); it2 != mObjectsCollidableMap.end(); ++it2)
		{
			const std::pair<const std::shared_ptr<GameObject>&, const std::shared_ptr<ICollidableObject>&> pair1 = *it1;
			const std::pair<const std::shared_ptr<GameObject>&, const std::shared_ptr<ICollidableObject>&> pair2 = *it2;

			const std::shared_ptr<GameObject>& pGO1 = pair1.first;
			const std::shared_ptr<GameObject>& pGO2 = pair2.first;
			const std::shared_ptr<ICollidableObject>& pCO1 = pair1.second;
			const std::shared_ptr<ICollidableObject>& pCO2 = pair2.second;

			// Check that both objects exist, are collidable, and are active; also don't check map against itself
			if (!pGO1 || !pGO2 
				|| !pCO1 || !pCO2
				|| !pGO1->IsActive() || !pGO2->IsActive() 
				|| (pGO1->GetObjectType() == ObjectType::mapTileCollidable && pGO2->GetObjectType() == ObjectType::mapTileCollidable)
				)
				continue;

			std::shared_ptr<IShape2D> shape1 = pCO1->GetShape().lock();
			std::shared_ptr<IShape2D> shape2 = pCO2->GetShape().lock();

			// Check that both objects have shapes.
			if (!shape1 || !shape2)
				continue;

			if (shape1->Intersects(*(shape2.get())))
			{
				pCO1->HandleCollision(pGO2);
				pCO2->HandleCollision(pGO1);
			}
		}
	}
}

void ObjectManager::RemoveDeletedObjects()
{
	for (std::shared_ptr<GameObject>& rpGameObject : mGameObjectsList)
	{
		if (rpGameObject.get()->IsRemoved())
		{
			mObjectsCollidableMap.erase(rpGameObject);
			rpGameObject.reset();
		}
	}

	mGameObjectsList.remove(nullptr);
}