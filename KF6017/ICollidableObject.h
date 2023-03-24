#pragma once
#include <memory>

class IShape2D;
class GameObject;

class ICollidableObject
{
public: 
	virtual std::weak_ptr<IShape2D> GetShape() const = 0;
	virtual void HandleCollision(std::shared_ptr<GameObject> pOtherObject) = 0;
};