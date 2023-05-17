#pragma once
#include <memory>

class ServiceManager;
class Creature;

typedef int PictureIndex;

class Weapon : public std::enable_shared_from_this<Weapon>
{
public:
	virtual ~Weapon();

	virtual void Attack(const std::shared_ptr<Creature> pAttacker, float attackAngle) = 0;

	PictureIndex GetAttackSprite() const;
	int GetBaseAttackDamage() const;
	double GetBaseAttackDelay() const;
	float GetBaseAttackReach() const;
	float GetBaseAttackSpriteScale() const;

protected:
	Weapon(const std::weak_ptr<ServiceManager> pServiceManager, int baseAttackDamage, double baseAttackDelay, float baseAttackReach, float attackSpriteScale = 1.f, PictureIndex attackSprite = -1);

	std::weak_ptr<ServiceManager> mpServiceManager;

	void SetAttackSprite(PictureIndex pictureIndex);
	void SetBaseAttackDamage(int damage);
	void SetBaseAttackDelay(double delay);
	void SetBaseAttackReach(float reach);
	void SetBaseAttackSpriteScale(float scale);

private:
	int mAttackDamage;
	double mAttackDelay;
	float mAttackReach;
	PictureIndex mAttackSprite;
	float mAttackSpriteScale;
};