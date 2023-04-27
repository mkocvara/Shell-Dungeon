#pragma once
#include "MovingGameObject.h"
#include "ICollidableObject.h"
#include <memory>

class AngledRectangle2D;

class Bullet : public MovingGameObject, public ICollidableObject
{
	typedef MovingGameObject Super;

public:
	Bullet(std::weak_ptr<ServiceManager> pServiceManager);
	virtual ~Bullet();

	virtual void Initialise(Vector2D position, float angle, float scale) override;
	virtual ErrorType Update(double deltaTime) override;

	virtual std::weak_ptr<IShape2D> GetShape() const override;
	virtual void HandleCollision(std::shared_ptr<GameObject> pOtherObject) override;

protected:
	bool IsOffScreen();

	std::shared_ptr<AngledRectangle2D> mpBoundingShape;

private:
	const wchar_t* mRenderSpritePath = L"Assets/bullet.bmp";
};

