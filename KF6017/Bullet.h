#pragma once
#include "MovingGameObject.h"
#include "ICollidableObject.h"
#include <memory>

class AngledRectangle2D;

class Bullet : public MovingGameObject, public ICollidableObject
{
public:
	Bullet(std::weak_ptr<ServiceManager> serviceManager);
	virtual ~Bullet();

	typedef MovingGameObject Super;

	virtual void Initialise(Vector2D position, float angle, float scale) override;
	virtual ErrorType Update(double deltaTime) override;

	virtual std::weak_ptr<IShape2D> GetShape() const override;
	virtual void HandleCollision(std::shared_ptr<GameObject> otherObject) override;

protected:
	virtual void Move(double deltaTime) override;
	bool IsOffScreen();

	std::shared_ptr<AngledRectangle2D> boundingShape;

private:
	const wchar_t* renderSpritePath = L"Assets/bullet.bmp";
	const float speed = 10.f;
};

