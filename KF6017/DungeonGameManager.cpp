#include "DungeonGameManager.h"
#include "ErrorLogger.h"
#include "ServiceManager.h"
#include "mydrawengine.h"
#include "GameObjectFactory.h"
#include "GameObject.h"
#include "ObjectManager.h"
#include "Event.h"


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

	mTimer += deltaTime;
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

	if (level == 1)
	{
		pObjectFactory->Create(ObjectType::knight, mpServiceManager);
	}
	else if (level == 2)
	{
		std::shared_ptr<GameObject> knight = pObjectFactory->Create(ObjectType::knight, mpServiceManager).lock();
		knight->SetPosition(300.0f, 500.0f);
	}
	else
	{
		pObjectFactory->Create(ObjectType::knight, mpServiceManager);
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
