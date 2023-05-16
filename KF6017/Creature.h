#pragma once
#include "MovingGameObject.h"
#include "ICollidableObject.h"
#include "Shapes.h"
#include <list>

class Weapon;

class Creature : public MovingGameObject, public ICollidableObject
{
	typedef MovingGameObject Super;
	
public:
	virtual ~Creature();

	virtual ErrorType Update(double deltaTime) override;

	int GetCurrentHealth() const;
	virtual void Damage(int damageAmount);
	virtual void Heal(int healAmount);
	int GetMaxHealth() const;
	void SetMaxHealth(int newMaxHealth);

	void EquipWeapon(const std::shared_ptr<Weapon> pWeapon);
	std::shared_ptr<Weapon> GetEquippedWeapon() const;
	bool IsAttackOnCooldown() const;

	virtual void HandleCollision(const std::shared_ptr<GameObject> pOtherObject) override;

protected:
	Creature(const std::weak_ptr<ServiceManager> pServiceManager);

	virtual ErrorType Render() override;

	virtual void Attack();
	virtual void Die();

	virtual void Move(double deltaTime) override;

	void SetShowHealthBar(bool shouldShowHealthBar);

private:
	int mMaxHealth = 1;
	int mHealth = mMaxHealth;
	Rectangle2D mHealthBar;
	bool mShowHealthBar = true;
	std::shared_ptr<Weapon> mpEquippedWeapon;
	float mAttackCooldown = 0.f;

	std::list<std::weak_ptr<GameObject>> mCollidedObstacles;
};

