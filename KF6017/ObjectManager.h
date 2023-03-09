#pragma once
#include <memory>
#include <list>
#include "errortype.h"

class GameObject;

class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();

	ErrorType UpdateAll(double deltaTime);
	void AddObject(GameObject* newObject);
	void AddObject(std::unique_ptr<GameObject>& newObject);
	void Clear();
	int GetNumberOfObjects() const;

private:
	void RemoveDeletedObjects();
	std::list<std::unique_ptr<GameObject>> gameObjects;
};

