#pragma once
#include "GameObject.h"

class MovingGameObject : public GameObject
{
public:
	virtual ~MovingGameObject();

	typedef GameObject Super;

	virtual ErrorType Update(double deltaTime) override;

protected:
	MovingGameObject(std::weak_ptr<ServiceManager> serviceManager, float friction = 0.f);

	// In units per second.
	Vector2D velocity;

	virtual void Move(double deltaTime);

private:
	// Fraction of velocity per second which gets applied in the opposite direction. Must be negative.
	float friction = 0.f;
};

