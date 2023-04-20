#pragma once
#include "MovingGameObject.h"
#include "ICollidableObject.h"
#include <memory>

class AngledRectangle2D;

class Spaceship : public MovingGameObject, public ICollidableObject
{
	typedef MovingGameObject Super;

public:
	Spaceship(std::weak_ptr<ServiceManager> pServiceManager);
	virtual ~Spaceship(); 

	virtual void Initialise(Vector2D position = Vector2D(0, 0), float angle = 0, float scale = 1) override;
	virtual ErrorType Update(double deltaTime) override;

	virtual std::weak_ptr<IShape2D> GetShape() const override;
	virtual void HandleCollision(std::shared_ptr<GameObject> pOtherObject) override;

protected:	
	const float mRotationSpeed = 2.f; 
	const float mAcceleration = 5.f;
	// mFriction = -0.5; see constructor

	virtual void HandleInputs(double deltaTime);

	void Shoot();

	std::shared_ptr<AngledRectangle2D> mpBoundingShape;

	void Die();

private:
	const wchar_t* mRenderSpritePath = L"Assets/basic.bmp";
};

