/*
* MovingGameObject.h
* 19002175
*
* Abstract subclass of GameObject objects that can move within the game world.
*/

#pragma once
#include "GameObject.h"

class MovingGameObject : public GameObject
{
	typedef GameObject Super;

public:
	virtual ~MovingGameObject();

	virtual ErrorType Update(double deltaTime) override;

	float GetMovementSpeed() const;
	float GetTimeToFullSpeed() const;
	float GetTimeToStop() const;

	void SetMovementSpeed(float speed);
	void SetTimeToFullSpeed(float time);
	void SetTimeToStop(float time);

	Vector2D GetVelocity();

protected:
	MovingGameObject(std::weak_ptr<ServiceManager> pServiceManager);

	// direction vector - always gets normalised before being used
	Vector2D mMoveDirection;

	virtual void Move(double deltaTime);
	void SetVelocity(const Vector2D& newVelocity);

private:
	// In units per second
	Vector2D mCurrentVelocity;
	float mMaxMovementSpeed = 10.f;
	
	// In seconds
	float mTimeToFullSpeed = 0.1f;
	float mTimeToStop = 0.2f;
};

