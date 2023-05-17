#include "Creature.h"
#include "winerror.h"
#include <algorithm>
#include <cmath>

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
		mCollidedObstacles.push_back(pOtherObject);
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

void Creature::Move(double deltaTime)
{
	// Remove movement in the direction of each collided object.
	for (const std::weak_ptr<GameObject>& collidedObject : mCollidedObstacles)
	{
		const std::shared_ptr<GameObject> collidedObjectLocked = collidedObject.lock();
		const Vector2D colObjPosition = collidedObjectLocked->GetPosition();
		const Vector2D colObjToThis = mPosition - colObjPosition;
		
		Vector2D collisionSideNormal;

		const float marginOfError = 15.f;

		if (std::abs(std::abs(colObjToThis.XValue) - std::abs(colObjToThis.YValue)) < marginOfError) // adjusts for small error caused by sinking slightly into walls on first contact
			collisionSideNormal.set(1, 1);
		else if (colObjToThis.XValue > std::abs(colObjToThis.YValue))	// right of tile
			collisionSideNormal.set(1, 0);
		else if (std::abs(colObjToThis.XValue) < colObjToThis.YValue)	// above tile
			collisionSideNormal.set(0, 1);
		else if (colObjToThis.XValue <= -std::abs(colObjToThis.YValue))	// left of tile
			collisionSideNormal.set(-1, 0);
		else if (-std::abs(colObjToThis.XValue) >= colObjToThis.YValue)	// below tile
			collisionSideNormal.set(0, -1);

		const Vector2D velocity = GetVelocity(); 
		if (collisionSideNormal * velocity < 0)
		{
			const Vector2D moveMultiplier = collisionSideNormal.perpendicularVector();
			const Vector2D newVelocity(std::abs(moveMultiplier.XValue) * velocity.XValue, std::abs(moveMultiplier.YValue) * velocity.YValue);
			SetVelocity(newVelocity);
		}
	}

	mCollidedObstacles.clear();

	Super::Move(deltaTime);
}

void Creature::SetShowHealthBar(bool shouldShowHealthBar)
{
	mShowHealthBar = shouldShowHealthBar;
}