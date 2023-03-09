#pragma once
#include "GameObject.h"

class MovingGameObject : public GameObject
{
public:
	virtual ~MovingGameObject();

	typedef GameObject Super;

	virtual ErrorType Update(double deltaTime) override;

protected:
	MovingGameObject(std::weak_ptr<ServiceManager> serviceManager);
	Vector2D velocity;
	virtual void Move(double deltaTime);
};

