#pragma once
#include <memory>

class ServiceManager;
class GameObject;

typedef int PictureIndex;

class Weapon : public std::enable_shared_from_this<Weapon>
{
public:
	virtual ~Weapon();

	virtual void Attack(const std::shared_ptr<GameObject> pAttacker, float attackAngle) = 0;

	PictureIndex GetAttackSprite() const;
	int GetBaseAttackDamage() const;
	double GetBaseAttackDelay() const;
	float GetBaseAttackReach() const;

protected:
	Weapon(const std::weak_ptr<ServiceManager> pServiceManager, PictureIndex attackSprite = -1, int baseAttackDamage = 1, double baseAttackDelay = 1.0, float baseAttackReach = 5.f);

	std::weak_ptr<ServiceManager> mpServiceManager;

	void SetAttackSprite(PictureIndex pictureIndex);
	void SetBaseAttackDamage(int damage);
	void SetBaseAttackDelay(double delay);
	void SetBaseAttackReach(float reach);

private:
	int mBaseAttackDamage;
	double mBaseAttackDelay;
	PictureIndex mAttackSprite;
	float mBaseAttackReach;
};