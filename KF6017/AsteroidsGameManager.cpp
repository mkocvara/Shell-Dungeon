#include "AsteroidsGameManager.h"
#include "ErrorLogger.h"
#include "ServiceManager.h"
#include "mydrawengine.h"
#include "GameObjectFactory.h"
#include "GameObject.h"
#include "ObjectManager.h"
#include "Event.h"
#include "GameObjectFactory.h"


// PUBLIC

AsteroidsGameManager::AsteroidsGameManager(std::shared_ptr<ServiceManager> pServiceManager) 
	: Super(pServiceManager)
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
		pObjectFactory->Create(ObjectType::spaceship);

		mAsteroidsLeft = 6;
		for (int i = 0; i < mAsteroidsLeft; i++)
		{
			pObjectFactory->Create(ObjectType::spacerock);
		}
	}
	else if (level == 2)
	{
		std::shared_ptr<GameObject> spaceship = pObjectFactory->Create(ObjectType::spaceship).lock();
		spaceship->SetPosition(300.0f, 500.0f);

		mAsteroidsLeft = 22;
		for (int i = 0; i < mAsteroidsLeft; i++)
		{
			pObjectFactory->Create(ObjectType::spacerock);
		}
	}
	else
	{
		pObjectFactory->Create(ObjectType::spaceship);
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
	else if (rEvent.type == EventType::mouseClick)
	{
		mLastClickLocation = rEvent.location;

		// DEBUG move camera
		/*std::shared_ptr<ServiceManager> pServiceManagerLocked = mpServiceManager.lock();
		std::shared_ptr<MyDrawEngine> pDrawEngine = pServiceManagerLocked->GetDrawEngine().lock();
		pDrawEngine->mpCamera->PlaceAt(mLastClickLocation);*/

		// DEBUG create explosion
		/*std::shared_ptr<ServiceManager> pServiceManagerLocked = mpServiceManager.lock();
		std::shared_ptr<GameObjectFactory> pObjectFactory = pServiceManagerLocked->GetObjectFactory().lock();
		pObjectFactory->Create(ObjectType::explosion, mpServiceManager, true, mLastClickLocation);*/
	}
}


// PROTECTED

void AsteroidsGameManager::Render()
{
	const std::shared_ptr<ServiceManager> pServiceManagerLocked = mpServiceManager.lock();

	const std::shared_ptr<MyDrawEngine> pDrawEngine = pServiceManagerLocked->GetDrawEngine().lock();
	if (!pDrawEngine)
	{
		ErrorLogger::Writeln(L"AsteroidsGameManager failed to retreive draw engine.");
		return;
	}

	ErrorType err = SUCCESS;
	const std::wstring levelText = L"Level " + std::to_wstring(mCurrentLevel);
	const std::wstring asteroidsText = L"Asteroids left: " + std::to_wstring(mAsteroidsLeft);
	const std::wstring timerText = L"Time: " + std::to_wstring((int)mTimer); // truncates the time and removes decimals
	const std::wstring clickText = L"Last clicked location: [" + std::to_wstring((int)mLastClickLocation.XValue) + L"; " + std::to_wstring((int)mLastClickLocation.YValue) + L"]";

	if(FAILED(pDrawEngine->WriteText(150, 150, levelText.c_str(), MyDrawEngine::GREEN)))
		err = FAILURE;
	if (FAILED(pDrawEngine->WriteText(150, 200, asteroidsText.c_str(), MyDrawEngine::WHITE)))
		err = FAILURE;
	if (FAILED(pDrawEngine->WriteText(150, 250, timerText.c_str(), MyDrawEngine::YELLOW)))
		err = FAILURE;
	if (FAILED(pDrawEngine->WriteText(150, 300, clickText.c_str(), MyDrawEngine::BLUE)))
		err = FAILURE;
	
	if (FAILED(err))
	{
		ErrorLogger::Writeln(L"Draw error in AsteroidsGameManager::Render().");
	}
}