#include "MovingGameObject.h"

MovingGameObject::MovingGameObject() : Super()
{
}

MovingGameObject::~MovingGameObject()
{
}

ErrorType MovingGameObject::Update(double deltaTime)
{
	if (!IsActive())
		return SUCCESS;

	Move(deltaTime);
	return Super::Update(deltaTime);
}

