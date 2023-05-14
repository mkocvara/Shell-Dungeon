#pragma once
#include "Enemy.h"

class Rectangle2D;

class Orc : public Enemy
{
	typedef Enemy Super;

public:
	Orc(const std::weak_ptr<ServiceManager> pServiceManager);
	virtual ~Orc();

	virtual void Initialise(Vector2D position, float angle, float scale) override;
	virtual ErrorType Update(double deltaTime) override;

	virtual std::weak_ptr<IShape2D> GetShape() const override;

	virtual void Attack() override;

protected:
	std::shared_ptr<Rectangle2D> mpBoundingShape;

private:
	const wchar_t* mRenderSpritePath = L"Assets/Enemies/Orcs/Orc Warrior/walk_down1.png";
	const float mBaseSpriteScale = 1.5f;
	const int mMaxHP = 12;

	Vector2D mAttackDirection;
};