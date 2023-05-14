#pragma once
#include "Creature.h"

class Rectangle2D;

class Knight final : public Creature
{
	typedef Creature Super;

public:
	Knight(const std::weak_ptr<ServiceManager> pServiceManager);
	virtual ~Knight();

	virtual void Initialise(Vector2D position, float angle, float scale) override;
	virtual ErrorType Update(double deltaTime) override;

	virtual std::weak_ptr<IShape2D> GetShape() const override;
	virtual void HandleCollision(const std::shared_ptr<GameObject> pOtherObject) override;
	
	std::weak_ptr<Rectangle2D> GetBounds() const;

protected:
	virtual void Attack() override;

private:
	const wchar_t* mRenderSpritePath = L"Assets/Knight/walk_down1.png";
	const float mBaseSpriteScale = 1.5f;

	std::shared_ptr<Rectangle2D> mpBoundingShape;

	void HandleInputs(double deltaTime);
};