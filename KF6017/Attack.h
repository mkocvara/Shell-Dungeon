/*
* Attack.h
* 19002175
*
* A moving and collidable game object representing a weapon attack (e.g., sword slash, club swing).
* Created by a weapon. Should be relatively easily expanded to allow moving ranged attacks.
*/

#pragma once
#include "MovingGameObject.h"
#include "ICollidableObject.h"
#include <memory>
#include <list>

class Weapon;
class AngledRectangle2D;
class Creature;

class Attack : public MovingGameObject, public ICollidableObject
{
	typedef MovingGameObject Super;

public:
	Attack(const std::weak_ptr<ServiceManager> pServiceManager);
	virtual ~Attack();

	virtual void Initialise(Vector2D position, float angle, float scale, PictureIndex sprite, const std::shared_ptr<Weapon> pSourceWeapon, const std::shared_ptr<Creature> pAttacker, float moveSpeed = 0.f);
	virtual ErrorType Update(double deltaTime) override;

	virtual std::weak_ptr<IShape2D> GetShape() const override;
	virtual void HandleCollision(const std::shared_ptr<GameObject> pOtherObject) override;

	int GetDamage() const;
	std::shared_ptr<GameObject> GetAttacker() const;

	void AddHitEntity(const std::weak_ptr<GameObject> pObject);
	bool HasEntityBeenHit(const std::weak_ptr<GameObject> pObject) const;

private:
	std::shared_ptr<Weapon> mpSourceWeapon;
	std::shared_ptr<Creature> mpAttacker;

	const double mDuration = 0.08f;
	double mTimer = 0.f;

	std::shared_ptr<AngledRectangle2D> mpBoundingShape;

	std::list<std::weak_ptr<GameObject>> mEntitiesHit;
};

