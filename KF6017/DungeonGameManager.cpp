#include "DungeonGameManager.h"
#include "ErrorLogger.h"
#include "ServiceManager.h"
#include "mydrawengine.h"
#include "GameObjectFactory.h"
#include "GameObject.h"
#include "ObjectManager.h"
#include "Event.h"

#include "Knight.h"
#include "Sword.h"

// PUBLIC

DungeonGameManager::DungeonGameManager(std::shared_ptr<ServiceManager> pServiceManager) : Super(pServiceManager)
{
}

DungeonGameManager::~DungeonGameManager()
{
}

void DungeonGameManager::Update(double deltaTime)
{
	Super::Update(deltaTime);

	mTimer += (float)deltaTime;
}

void DungeonGameManager::StartLevel(LevelId level)
{
	std::shared_ptr<ServiceManager> pServiceManagerLocked = mpServiceManager.lock();
	std::shared_ptr<GameObjectFactory> pObjectFactory = pServiceManagerLocked->GetObjectFactory().lock();

	if (!pObjectFactory)
	{
		ErrorLogger::Writeln(L"DungeonGameManager failed to retreive object factory.");
		return;
	}

	mGameState = GameState::alive;
	mTimer = 0.f;
	mCurrentLevel = level;

	const std::shared_ptr<GameObject> pKnight = pObjectFactory->Create(ObjectType::knight).lock();
	const std::shared_ptr<Knight> pKnightAsKnight = std::static_pointer_cast<Knight>(pKnight);
	mpPlayerKnight = pKnightAsKnight;

	if (level == 1)
	{
		pKnightAsKnight->EquipWeapon(std::make_shared<Sword>(mpServiceManager)); // May wish to implement a WeaponFactory when/if many more weapons are added.

		std::shared_ptr<GameObject> pOrc;
		pOrc = pObjectFactory->Create(ObjectType::orc).lock();
		pOrc->SetPosition(200.f, 200.f);
		pOrc = pObjectFactory->Create(ObjectType::orc).lock();
		pOrc->SetPosition(500.f, 700.f);
		pOrc = pObjectFactory->Create(ObjectType::orc).lock();
		pOrc->SetPosition(900.f, 800.f);
	}
	else if (level == 2)
	{
		std::shared_ptr<GameObject> knight = pObjectFactory->Create(ObjectType::knight).lock();
		knight->SetPosition(300.0f, 500.0f);
	}
	else
	{
		pObjectFactory->Create(ObjectType::knight);
	}
}

void DungeonGameManager::RestartLevel()
{
	std::shared_ptr<ServiceManager> pServiceManagerLocked = mpServiceManager.lock();
	std::shared_ptr<ObjectManager> pObjectManager = pServiceManagerLocked->GetObjectManager().lock();

	pObjectManager->Clear();

	StartLevel(mCurrentLevel);
}

void DungeonGameManager::EndLevel()
{
	std::shared_ptr<ServiceManager> pServiceManagerLocked = mpServiceManager.lock();
	std::shared_ptr<ObjectManager> pObjectManager = pServiceManagerLocked->GetObjectManager().lock();

	pObjectManager->Clear();
}

void DungeonGameManager::HandleEvent(const Event& rEvent)
{
	/*if (rEvent.type == EventType::asteroidDestroyed)
	{
		mAsteroidsLeft--;
	}*/
}

Vector2D DungeonGameManager::GetPlayerLocation() const
{
	return mpPlayerKnight.lock()->GetPosition();
}

std::weak_ptr<Rectangle2D> DungeonGameManager::GetPlayerBounds() const
{
	return mpPlayerKnight.lock()->GetBounds();
}


// PROTECTED

void DungeonGameManager::Render()
{
	std::shared_ptr<ServiceManager> pServiceManagerLocked = mpServiceManager.lock();

	std::shared_ptr<MyDrawEngine> pDrawEngine = pServiceManagerLocked->GetDrawEngine().lock();
	if (!pDrawEngine)
	{
		ErrorLogger::Writeln(L"DungeonGameManager failed to retreive draw engine.");
		return;
	}

	ErrorType err = SUCCESS;
	std::wstring levelText = L"Level " + std::to_wstring(mCurrentLevel);
	std::wstring timerText = L"Time: " + std::to_wstring((int)mTimer); // truncates the time and removes decimals

	if(FAILED(pDrawEngine->WriteText(150, 150, levelText.c_str(), MyDrawEngine::GREEN)))
		err = FAILURE;
	if (FAILED(pDrawEngine->WriteText(150, 200, timerText.c_str(), MyDrawEngine::YELLOW)))
		err = FAILURE;
	
	if (FAILED(err))
	{
		ErrorLogger::Writeln(L"Draw error in DungeonGameManager::Render().");
	}
}
