#include "MovingGameObject.h"
#include "ErrorLogger.h"

MovingGameObject::MovingGameObject(std::weak_ptr<ServiceManager> pServiceManager, float friction) : Super(pServiceManager)
{
	mFriction = friction;
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
	if (mFriction > 0.f)
		ErrorLogger::Writeln(L"Friction is not negative in moving object.");

	// apply friction
	Vector2D f = mFriction * mVelocity;
	mVelocity += f * deltaTime;

	mPosition += mVelocity * deltaTime;
}
