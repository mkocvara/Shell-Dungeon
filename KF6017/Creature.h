#pragma once
#include "MovingGameObject.h"
#include "ICollidableObject.h"
#include "Shapes.h"

class Weapon;

class Creature : public MovingGameObject, public ICollidableObject
{
	typedef MovingGameObject Super;
	
public:
	virtual ~Creature();

	int GetCurrentHealth() const;
	virtual void Damage(int damageAmount);
	virtual void Heal(int healAmount);
	int GetMaxHealth() const;
	void SetMaxHealth(int newMaxHealth);

	void EquipWeapon(const std::shared_ptr<Weapon> pWeapon);
	std::shared_ptr<Weapon> GetEquippedWeapon() const;

protected:
	Creature(const std::weak_ptr<ServiceManager> pServiceManager);

	virtual ErrorType Render() override;

	virtual void Attack() = 0;
	virtual void Die();

	void SetShowHealthBar(bool shouldShowHealthBar);

private:
	int mMaxHealth = 1;
	int mHealth = mMaxHealth;
	Rectangle2D mHealthBar;
	bool mShowHealthBar = true;
	std::shared_ptr<Weapon> mpEquippedWeapon;
};

