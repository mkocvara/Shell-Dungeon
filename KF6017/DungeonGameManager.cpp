#include "DungeonGameManager.h"
#include "ErrorLogger.h"
#include "Level.h"

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
	mActiveLevelId = level;

	const std::shared_ptr<GameObject> pKnight = pObjectFactory->Create(ObjectType::knight).lock();
	const std::shared_ptr<Knight> pKnightAsKnight = std::static_pointer_cast<Knight>(pKnight);
	pKnightAsKnight->EquipWeapon(std::make_shared<Sword>(mpServiceManager)); // May wish to implement a WeaponFactory when/if many more weapons are added.
	mpPlayerKnight = pKnightAsKnight;

	mActiveLevel = std::make_shared<Level>(mpServiceManager, level, pKnightAsKnight);
}

void DungeonGameManager::RestartLevel()
{
	EndLevel();
	StartLevel(mActiveLevelId);
}

void DungeonGameManager::EndLevel()
{
	std::shared_ptr<ServiceManager> pServiceManagerLocked = mpServiceManager.lock();
	std::shared_ptr<ObjectManager> pObjectManager = pServiceManagerLocked->GetObjectManager().lock();

	mActiveLevel.reset();
	pObjectManager->Clear();
}

void DungeonGameManager::HandleEvent(const Event& rEvent)
{
	// TEMPORARY
	/*if (rEvent.type == EventType::mouseClick)
	{
		std::shared_ptr<ServiceManager> pServiceManagerLocked = mpServiceManager.lock();
		std::shared_ptr<GameObjectFactory> pObjectFactory = pServiceManagerLocked->GetObjectFactory().lock();

		std::shared_ptr<GameObject> tile = pObjectFactory->Create(ObjectType::mapTileCollidable, true, rEvent.location).lock();
		tile->SetRenderSprite(L"Assets/Environment/Floor/floor.png");
	}*/
	// ---------
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
	std::wstring levelText = L"Level " + std::to_wstring(mActiveLevelId);
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
