#include "MovingGameObject.h"

MovingGameObject::MovingGameObject(std::weak_ptr<ServiceManager> serviceManager) : Super(serviceManager)
{
}


MovingGameObject::~MovingGameObject()
{
}


// PUBLIC

ErrorType MovingGameObject::Update(double deltaTime)
{
	if (!IsActive())
		return SUCCESS;

	Move(deltaTime);
	return Super::Update(deltaTime);
}


// PROTECTED

void MovingGameObject::Move(double deltaTime)
{
	position += velocity;
}
