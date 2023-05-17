#include "Orc.h"
#include "ObjectType.h"
#include "RandUtil.h"

#include "ServiceManager.h"
#include "mydrawengine.h"
#include "DungeonGameManager.h"

#include "Club.h"


namespace {
	float baseMovementSpeed = 6.f;
	float baseTimeToFullSpeed = 0.03f;
	float baseTimeToStop = 0.03f;
	
	double idleMovementDirectionChangeInterval = 5.0; // only if player died
}


// PUBLIC

Orc::Orc(const std::weak_ptr<ServiceManager> pServiceManager)
	: Super(pServiceManager)
{
	mObjectType = ObjectType::orc;
}

Orc::~Orc()
{
}

void Orc::Initialise(Vector2D position, float angle, float scale)
{
	scale *= mBaseSpriteScale;

	SetRenderSprite(mRenderSpritePath);

	SetMaxHealth(mMaxHP);
	Heal(mMaxHP);
	SetShowHealthBar(true);

	const std::shared_ptr<MyDrawEngine> pDrawEngine = mpServiceManager.lock()->GetDrawEngine().lock();
	int spriteHeight, spriteWidth;
	pDrawEngine->GetDimensions(mRenderSprite, spriteHeight, spriteWidth);
	mpBoundingShape = std::make_shared<Rectangle2D>();
	mpBoundingShape->PlaceAt(position, (float)spriteHeight * scale, (float)spriteWidth * scale);
	
	SetMovementSpeed(baseMovementSpeed);
	SetTimeToFullSpeed(baseTimeToFullSpeed);
	SetTimeToStop(baseTimeToStop);

	EquipWeapon(std::shared_ptr<Weapon>(new Club(mpServiceManager)));

	Super::Initialise(position, angle, scale);
}

ErrorType Orc::Update(double deltaTime)
{
	if (!IsActive())
		return SUCCESS;

	mpBoundingShape->SetCentre(mPosition);

	const std::shared_ptr<GameManager> pGameManager = mpServiceManager.lock()->GetGameManager().lock();
	const std::shared_ptr<DungeonGameManager> pDungeonGameManager = std::static_pointer_cast<DungeonGameManager>(pGameManager);
	
	if (pDungeonGameManager->GetGameState() == GameState::playerAlive)
	{
		const Vector2D playerPosition = pDungeonGameManager->GetPlayerLocation();
		const Vector2D orcToPlayer = playerPosition - mPosition;
		const std::shared_ptr<Weapon> equippedWeapon = GetEquippedWeapon();

		if (equippedWeapon)
		{
			const float attackRange = GetEquippedWeapon()->GetBaseAttackReach();
			const float shapeOffset = pDungeonGameManager->GetPlayerBounds().lock()->GetWidth() / 2;
			const float attackDistanceVariability = (float)RandUtil::randRangeDouble(-attackRange / 4, attackRange / 4);

			if (orcToPlayer.magnitude() <= attackRange + shapeOffset + attackDistanceVariability)
			{
				mMoveDirection.set(0, 0);
				mAttackDirection = orcToPlayer;
				Attack();
			}
			else if (!IsAttackOnCooldown())
			{
				mMoveDirection = orcToPlayer;
			}
		}
	}
	else // currently only reached if player is dead
	{
		SetMovementSpeed(baseMovementSpeed / 2);
		static double interval = idleMovementDirectionChangeInterval;
		interval += deltaTime;
		if (interval > idleMovementDirectionChangeInterval)
		{
			interval -= idleMovementDirectionChangeInterval;
			mMoveDirection = RandUtil::randDirectionVector();
		}

	}
	

	return Super::Update(deltaTime);
}

std::weak_ptr<IShape2D> Orc::GetShape() const
{
	return mpBoundingShape;
}

void Orc::Attack()
{
	if (IsAttackOnCooldown())
		return;

	const std::shared_ptr<Weapon> pWeapon = GetEquippedWeapon();
	pWeapon->Attack(std::static_pointer_cast<Creature>(shared_from_this()), mAttackDirection.angle());

	Super::Attack();
}