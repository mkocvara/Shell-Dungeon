#include "Creature.h"
#include "winerror.h"
#include <algorithm>

#define NOMINMAX

#include "ServiceManager.h"
#include "mydrawengine.h"

#include "Weapon.h"


Creature::Creature(const std::weak_ptr<ServiceManager> pServiceManager)
	: Super(pServiceManager)
{
}


// PUBLIC

Creature::~Creature()
{
}

ErrorType Creature::Update(double deltaTime)
{
	if (IsAttackOnCooldown())
		mAttackCooldown -= deltaTime;

	return Super::Update(deltaTime);
}

int Creature::GetCurrentHealth() const
{
	return mHealth;
}

void Creature::Damage(int damageAmount)
{
	mHealth = std::max(0, mHealth - damageAmount);

	if (mHealth == 0)
		Die();
}

void Creature::Heal(int healAmount)
{
	mHealth = std::min(mMaxHealth, mHealth + healAmount);
}

int Creature::GetMaxHealth() const
{
	return mMaxHealth;
}

void Creature::SetMaxHealth(int newMaxHealth)
{
	mMaxHealth = newMaxHealth;
}

void Creature::EquipWeapon(const std::shared_ptr<Weapon> pWeapon)
{
	mpEquippedWeapon = pWeapon;
}

std::shared_ptr<Weapon> Creature::GetEquippedWeapon() const
{
	return mpEquippedWeapon;
}

bool Creature::IsAttackOnCooldown() const
{
	return mAttackCooldown > 0.f;
}

void Creature::HandleCollision(const std::shared_ptr<GameObject> pOtherObject)
{
	if (pOtherObject->GetObjectType() == ObjectType::mapTileCollidable)
	{
		RevertMove(); // TODO: Test if this is reliable - in the current state, it will only work if it is called after Move(); 
							//	Does not work well - MUST BE REWORKED LATER (finish levels first)
	}
}


// PROTECTED

ErrorType Creature::Render()
{
	if (FAILED(Super::Render()))
		return FAILURE;

	// Render health bar
	if (mShowHealthBar)
	{
		// This is in Render in case different sized sprites are used for any single Creature subclass
		const std::shared_ptr<MyDrawEngine> pDrawEngine = mpServiceManager.lock()->GetDrawEngine().lock();
		int spriteHeight, spriteWidth;
		pDrawEngine->GetDimensions(mRenderSprite, spriteHeight, spriteWidth);

		spriteHeight *= mRenderScale;
		spriteWidth *= mRenderScale;

		const float healthBarHeight = (spriteHeight / 2) + (10.f * mRenderScale);
		mHealthBar.PlaceAt(mPosition + Vector2D(0.f, healthBarHeight), spriteHeight / 20.f, spriteWidth * ((float)mHealth / (float)mMaxHealth));
		pDrawEngine->FillRect(mHealthBar, MyDrawEngine::GREEN);
	}

	return SUCCESS;
}

void Creature::Attack()
{
	if (!IsAttackOnCooldown())
		mAttackCooldown = mpEquippedWeapon->GetBaseAttackDelay();
}

void Creature::Die()
{
	Remove();
}

void Creature::SetShowHealthBar(bool shouldShowHealthBar)
{
	mShowHealthBar = shouldShowHealthBar;
}