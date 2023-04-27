#pragma once
#include "MovingGameObject.h"
#include "ICollidableObject.h"

class Rectangle2D;

class Knight : public MovingGameObject, public ICollidableObject
{
	typedef MovingGameObject Super;

public:
	Knight(std::weak_ptr<ServiceManager> pServiceManager);
	virtual ~Knight();

	virtual void Initialise(Vector2D position, float angle, float scale) override;
	virtual ErrorType Update(double deltaTime) override;

	virtual std::weak_ptr<IShape2D> GetShape() const override;
	virtual void HandleCollision(std::shared_ptr<GameObject> pOtherObject) override;

protected:
	std::shared_ptr<Rectangle2D> mpBoundingShape;

	virtual void HandleInputs(double deltaTime);
	void Attack();
	void Die();

private:
	const wchar_t* mRenderSpritePath = L"Assets/Knight/walk_down1.png";
	const float mBaseSpriteScale = 2.f;
};

