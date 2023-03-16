#pragma once
#include <memory>
#include <list>
#include <map>
#include "errortype.h"

class GameObject;
class ICollidableObject;

class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();

	ErrorType UpdateAll(double deltaTime);
	void AddObject(std::shared_ptr<GameObject>& newObject);
	void AddCollidableObject(std::shared_ptr<GameObject>& asGameObject, std::shared_ptr<ICollidableObject>& asCollidableObject);
	void Clear();
	int GetNumberOfObjects() const;

private:
	void CheckCollisions();
	void RemoveDeletedObjects();
	std::list<std::shared_ptr<GameObject>> gameObjects;
	std::map<std::shared_ptr<GameObject>, std::shared_ptr<ICollidableObject>> collidableObjectsLookup;

	//std::list<std::shared_ptr<ICollidableObject>> collidableObjects;
};

