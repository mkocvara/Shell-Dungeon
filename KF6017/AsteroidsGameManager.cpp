#include "AsteroidsGameManager.h"
#include "ErrorLogger.h"
#include "ServiceManager.h"
#include "mydrawengine.h"
#include "GameObjectFactory.h"
#include "GameObject.h"
#include "ObjectManager.h"
#include "Event.h"


// PUBLIC

AsteroidsGameManager::AsteroidsGameManager(std::shared_ptr<ServiceManager> pServiceManager) : Super(pServiceManager)
{
}

AsteroidsGameManager::~AsteroidsGameManager()
{
}

void AsteroidsGameManager::Update(double deltaTime)
{
	Super::Update(deltaTime);

	if (mAsteroidsLeft == 0)
	{
		EndLevel();

		if (mCurrentLevel == 2)
			StartLevel(1);
		else
			StartLevel(mCurrentLevel+1);
	}

	mTimer += deltaTime;
}

void AsteroidsGameManager::StartLevel(LevelId level)
{
	std::shared_ptr<ServiceManager> pServiceManagerLocked = mpServiceManager.lock();
	std::shared_ptr<GameObjectFactory> pObjectFactory = pServiceManagerLocked->GetObjectFactory().lock();

	if (!pObjectFactory)
	{
		ErrorLogger::Writeln(L"AsteroidsGameManager failed to retreive object factory.");
		return;
	}

	mGameState = GameState::alive;
	mTimer = 0.f;
	mCurrentLevel = level;

	if (level == 1)
	{
		pObjectFactory->Create(ObjectType::spaceship, mpServiceManager);

		mAsteroidsLeft = 6;
		for (int i = 0; i < mAsteroidsLeft; i++)
		{
			pObjectFactory->Create(ObjectType::spacerock, mpServiceManager);
		}
	}
	else if (level == 2)
	{
		std::shared_ptr<GameObject> spaceship = pObjectFactory->Create(ObjectType::spaceship, mpServiceManager).lock();
		spaceship->SetPosition(300.0f, 500.0f);

		mAsteroidsLeft = 22;
		for (int i = 0; i < mAsteroidsLeft; i++)
		{
			pObjectFactory->Create(ObjectType::spacerock, mpServiceManager);
		}
	}
	else
	{
		pObjectFactory->Create(ObjectType::spaceship, mpServiceManager);
	}
}

void AsteroidsGameManager::RestartLevel()
{
	std::shared_ptr<ServiceManager> pServiceManagerLocked = mpServiceManager.lock();
	std::shared_ptr<ObjectManager> pObjectManager = pServiceManagerLocked->GetObjectManager().lock();

	pObjectManager->Clear();

	StartLevel(mCurrentLevel);
}

void AsteroidsGameManager::EndLevel()
{
	std::shared_ptr<ServiceManager> pServiceManagerLocked = mpServiceManager.lock();
	std::shared_ptr<ObjectManager> pObjectManager = pServiceManagerLocked->GetObjectManager().lock();

	pObjectManager->Clear();
}

void AsteroidsGameManager::HandleEvent(const Event& rEvent)
{
	if (rEvent.type == EventType::asteroidDestroyed)
	{
		mAsteroidsLeft--;
	}
}


// PROTECTED

void AsteroidsGameManager::Render()
{
	std::shared_ptr<ServiceManager> pServiceManagerLocked = mpServiceManager.lock();

	std::shared_ptr<MyDrawEngine> pDrawEngine = pServiceManagerLocked->GetDrawEngine().lock();
	if (!pDrawEngine)
	{
		ErrorLogger::Writeln(L"AsteroidsGameManager failed to retreive draw engine.");
		return;
	}

	ErrorType err = SUCCESS;
	std::wstring levelText = L"Level " + std::to_wstring(mCurrentLevel);
	std::wstring asteroidsText = L"Asteroids left: " + std::to_wstring(mAsteroidsLeft);
	std::wstring timerText = L"Time: " + std::to_wstring((int)mTimer); // truncates the time and removes decimals

	if(FAILED(pDrawEngine->WriteText(150, 150, levelText.c_str(), MyDrawEngine::GREEN)))
		err = FAILURE;
	if (FAILED(pDrawEngine->WriteText(150, 200, asteroidsText.c_str(), MyDrawEngine::WHITE)))
		err = FAILURE;
	if (FAILED(pDrawEngine->WriteText(150, 250, timerText.c_str(), MyDrawEngine::YELLOW)))
		err = FAILURE;
	
	if (FAILED(err))
	{
		ErrorLogger::Writeln(L"Draw error in AsteroidsGameManager::Render().");
	}
}
