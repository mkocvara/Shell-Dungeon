#include "MovingGameObject.h"
#include "ErrorLogger.h"

MovingGameObject::MovingGameObject(std::weak_ptr<ServiceManager> serviceManager, float friction) : Super(serviceManager)
{
	this->friction = friction;
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
	if (friction < 0.f)
		ErrorLogger::Writeln(L"Friction is not negative in: "); // TODO add type

	// apply friction
	Vector2D f = friction * velocity;
	velocity += f * deltaTime;

	position += velocity * deltaTime;
}
