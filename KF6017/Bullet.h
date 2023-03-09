#pragma once
#include "MovingGameObject.h"

class Bullet : public MovingGameObject
{
public:
	Bullet(std::weak_ptr<ServiceManager> serviceManager);
	virtual ~Bullet();

	typedef MovingGameObject Super;

	virtual void Initialise(Vector2D position, float angle, float scale) override;
	virtual ErrorType Update(double deltaTime) override;

protected:
	virtual void Move(double deltaTime) override;
	bool IsOffScreen();

private:
	const wchar_t* renderSpritePath = L"Assets/bullet.bmp";
	const float speed = 10.f;
};

