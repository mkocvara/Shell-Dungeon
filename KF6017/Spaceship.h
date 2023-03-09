#pragma once
#include "MovingGameObject.h"
#include <memory>

class Spaceship : public MovingGameObject
{
public:
	Spaceship(std::weak_ptr<ServiceManager> serviceManager);
	virtual ~Spaceship(); 

	typedef MovingGameObject Super;

	virtual void Initialise(Vector2D position = Vector2D(0, 0), float angle = 0, float scale = 1) override;
	virtual ErrorType Update(double deltaTime) override;

protected:	
	const float rotationSpeed = 2.f; 
	const float acceleration = 5.f;
	const float friction = -0.5f;

	//virtual void Move(double deltaTime) override;
	virtual void HandleInputs(double deltaTime);

	void Shoot();

private:
	const wchar_t* renderSpritePath = L"Assets/basic.bmp";
};

