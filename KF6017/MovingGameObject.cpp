#include "MovingGameObject.h"
#include "ErrorLogger.h"
#include <cmath>

MovingGameObject::MovingGameObject(std::weak_ptr<ServiceManager> pServiceManager) 
	: Super(pServiceManager)
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

void MovingGameObject::SetMovementSpeed(float speed)
{
	mMaxMovementSpeed = speed;
}

void MovingGameObject::SetTimeToFullSpeed(float time)
{
	mTimeToFullSpeed = time;
}

void MovingGameObject::SetTimeToStop(float time)
{
	mTimeToStop = time;
}

void MovingGameObject::RevertMove()
{
	// TODO: Expand this so that it only reverts move component in the direction of collided object.
	mPosition -= mCurrentVelocity;
}


// PROTECTED

void MovingGameObject::Move(double deltaTime)
{
	mMoveDirection = mMoveDirection.unitVector();

	const bool isAccelerating = mMoveDirection.magnitude() != 0;
	const Vector2D desiredVelocity = isAccelerating ? mMoveDirection * mMaxMovementSpeed : Vector2D(0, 0);
	const float timeToReachDesiredVelocity = (desiredVelocity.magnitude() == 0) ? mTimeToStop : mTimeToFullSpeed;

	if (timeToReachDesiredVelocity == 0)
	{
		mCurrentVelocity = desiredVelocity;
	}
	else
	{
		const float maxDistanceDelta = ((float)deltaTime / timeToReachDesiredVelocity) * mMaxMovementSpeed;
		Vector2D diffVelocity = desiredVelocity - mCurrentVelocity;
		float diffVelocityMagnitude = diffVelocity.magnitude();
		if (diffVelocityMagnitude <= maxDistanceDelta || diffVelocityMagnitude == 0)
			mCurrentVelocity = desiredVelocity;
		else
			mCurrentVelocity = mCurrentVelocity + diffVelocity / diffVelocityMagnitude * maxDistanceDelta;
	}

	mPosition += mCurrentVelocity;
}
