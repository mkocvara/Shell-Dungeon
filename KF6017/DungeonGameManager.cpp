/*
* DungeonGameManager.cpp
* 19002175
*
* Game-specific service for managing levels and time. Handles transitions between levels, death, and victory.
* Keeps track of and gives the player Boons. Creates (as per level) and keeps a reference to the Knight and exposes some of its information.
*/

#include "DungeonGameManager.h"
#include "ErrorLogger.h"
#include "Level.h"
#include "Event.h"

#include "ServiceManager.h"
#include "mydrawengine.h"
#include "myinputs.h"
#include "GameObjectFactory.h"
#include "GameObject.h"
#include "ObjectManager.h"
#include "DungeonSFXManager.h"

#include "Knight.h"
#include "Sword.h"
#include "HealthBoon.h"
#include "DamageBoon.h"
#include "SpeedBoon.h"

// PUBLIC

DungeonGameManager::DungeonGameManager(std::shared_ptr<ServiceManager> pServiceManager) : Super(pServiceManager)
{
}

DungeonGameManager::~DungeonGameManager()
{
}

void DungeonGameManager::Update(double deltaTime)
{
	const bool continuePressed = ContinuePressed();

	switch (mGameState)
	{
	case GameState::playerDead:
	case GameState::victory:
		if (continuePressed)
			RestartGame();
		break;

	case GameState::levelCleared:
		if (HandleBoonSelection() && continuePressed)
		{
			EndLevel();
			StartLevel(mActiveLevelId + 1);
		}
		break;

	case GameState::playerAlive:
		mTimer += (float)deltaTime;
		break;
	}

	Super::Update(deltaTime);
}

void DungeonGameManager::StartLevel(LevelId level)
{
	mTimeAtLevelStart = mTimer;

	std::shared_ptr<ServiceManager> pServiceManagerLocked = mpServiceManager.lock();
	std::shared_ptr<GameObjectFactory> pObjectFactory = pServiceManagerLocked->GetObjectFactory().lock();

	mActiveLevelId = level;

	const std::shared_ptr<GameObject> pKnight = pObjectFactory->Create(ObjectType::knight).lock();
	const std::shared_ptr<Knight> pKnightAsKnight = std::static_pointer_cast<Knight>(pKnight);
	pKnightAsKnight->EquipWeapon(std::make_shared<Sword>(mpServiceManager)); // TODO May wish to implement a WeaponFactory when/if many more weapons are added.
	mpPlayerKnight = pKnightAsKnight;
	ApplyBoons(mpPlayerKnight);

	mActiveLevel = std::make_shared<Level>(mpServiceManager, level, pKnightAsKnight);
	mEnemiesRemaining = mActiveLevel->GetTotalEnemies();

	mGameState = GameState::playerAlive;

	const std::shared_ptr<SFXManager> pSFX = mpServiceManager.lock()->GetSFXManager().lock();
	if (typeid(*pSFX) == typeid(DungeonSFXManager))
	{
		std::shared_ptr<DungeonSFXManager> pSFXManager = std::static_pointer_cast<DungeonSFXManager>(pSFX);
		if (pSFXManager)
		{
			pSFXManager->PlaySwordUnsheath();
		}
	}
}

void DungeonGameManager::RestartLevel()
{
	EndLevel();
	mTimer = mTimeAtLevelStart;
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
	if (rEvent.type == EventType::enemyKilled
		&& --mEnemiesRemaining == 0)
	{
		if (mActiveLevelId + 1 > mNumLevels)
		{
			mGameState = GameState::victory;
		}
		else 
		{
			mGameState = GameState::levelCleared;
			const std::shared_ptr<SFXManager> pSFX = mpServiceManager.lock()->GetSFXManager().lock();
			if (typeid(*pSFX) == typeid(DungeonSFXManager))
			{
				std::shared_ptr<DungeonSFXManager> pSFXManager = std::static_pointer_cast<DungeonSFXManager>(pSFX);
				if (pSFXManager)
				{
					pSFXManager->PlayLevelCleared();
				}
			}
		}
	}
	if (rEvent.type == EventType::playerDied)
	{
		mGameState = GameState::playerDead;
		const std::shared_ptr<SFXManager> pSFX = mpServiceManager.lock()->GetSFXManager().lock();
		if (typeid(*pSFX) == typeid(DungeonSFXManager))
		{
			std::shared_ptr<DungeonSFXManager> pSFXManager = std::static_pointer_cast<DungeonSFXManager>(pSFX);
			if (pSFXManager)
			{
				pSFXManager->PlayKnightDeath();
			}
		}
	}
}

void DungeonGameManager::RestartGame()
{
	EndLevel();
	mTimer = 0.f;
	StartLevel(1);
	mGameState = GameState::playerAlive;
	mSelectedBoons.clear();
}

GameState DungeonGameManager::GetGameState() const
{
	return mGameState;
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
	const std::shared_ptr<ServiceManager> pServiceManagerLocked = mpServiceManager.lock();
	const std::shared_ptr<MyDrawEngine> pDrawEngine = pServiceManagerLocked->GetDrawEngine().lock();
	
	if (mGameState == GameState::playerAlive)
	{
		ErrorType err = SUCCESS;
		const std::wstring levelText = L"Level " + std::to_wstring(mActiveLevelId);
		const std::wstring timerText = L"Time: " + std::to_wstring((int)mTimer); // truncates the time and removes decimals
		const std::wstring enemiesText = L"Enemies remaining: " + std::to_wstring(mEnemiesRemaining);

		if(FAILED(pDrawEngine->WriteText(100, 100, levelText.c_str(), MyDrawEngine::GREEN)))
			err = FAILURE;
		if (FAILED(pDrawEngine->WriteText(100, 150, timerText.c_str(), MyDrawEngine::YELLOW)))
			err = FAILURE;
		if (FAILED(pDrawEngine->WriteText(100, 200, enemiesText.c_str(), MyDrawEngine::RED)))
			err = FAILURE;
	
		if (FAILED(err))
		{
			ErrorLogger::Writeln(L"Draw error in DungeonGameManager::Render().");
		}
	}
	else if (mGameState == GameState::levelCleared)
	{
		ErrorType err = SUCCESS;
		const std::wstring levelClearedText = L"You have prevailed and slain your enemies.";
		const std::wstring levelClearedText2 = L"The level is clear, but prepare for more...";
		const std::wstring bottomText = L"Level : " + std::to_wstring(mActiveLevelId) + L"  |  Time so far: " + std::to_wstring((int)mTimer);
		const std::wstring continueText = (BoonSelected()) ? L"Press [Spacebar] to continue." : L"Select a boon before proceeding:";
		const std::wstring boonText = L"1. Boon of Health     2. Boon of Strength     3. Boon of Speed";

		const int screenWidth = pDrawEngine->GetScreenWidth();
		const int screenHeight = pDrawEngine->GetScreenHeight();
		
		if (FAILED(pDrawEngine->WriteText(screenWidth / 2, screenHeight / 2 - 90, levelClearedText.c_str(), MyDrawEngine::CYAN, 0, true)))
			err = FAILURE;
		if (FAILED(pDrawEngine->WriteText(screenWidth / 2, screenHeight / 2 - 65, levelClearedText2.c_str(), MyDrawEngine::CYAN, 0, true)))
			err = FAILURE;
		if (FAILED(pDrawEngine->WriteText(screenWidth / 2, screenHeight / 2, bottomText.c_str(), MyDrawEngine::YELLOW, 0, true)))
			err = FAILURE;
		if (FAILED(pDrawEngine->WriteText(screenWidth / 2, screenHeight / 2 + 80, continueText.c_str(), MyDrawEngine::CYAN, 0, true)))
			err = FAILURE;
		if (!BoonSelected())
		{
			if (FAILED(pDrawEngine->WriteText(screenWidth / 2, screenHeight / 2 + 100, boonText.c_str(), MyDrawEngine::CYAN, 0, true)))
				err = FAILURE;
		}

		if (FAILED(err))
		{
			ErrorLogger::Writeln(L"Draw error in DungeonGameManager::Render().");
		}
	}
	else if (mGameState == GameState::playerDead)
	{
		ErrorType err = SUCCESS;
		const std::wstring levelClearedText = L"You were overwhelmed by your vile enemies. You are dead.";
		const std::wstring levelClearedText2 = L"But not to worry, there are always more adventurous knights trying their luck in the Dungeon.";
		const std::wstring bottomText = L"Level reached: " + std::to_wstring(mActiveLevelId) + L"  |  Time spent: " + std::to_wstring((int)mTimer);
		const std::wstring continueText = L"Press [Spacebar] to continue.";

		const int screenWidth = pDrawEngine->GetScreenWidth();
		const int screenHeight = pDrawEngine->GetScreenHeight();

		if (FAILED(pDrawEngine->WriteText(screenWidth / 2, screenHeight / 2 - 90, levelClearedText.c_str(), MyDrawEngine::RED, 0, true)))
			err = FAILURE;
		if (FAILED(pDrawEngine->WriteText(screenWidth / 2, screenHeight / 2 - 65, levelClearedText2.c_str(), MyDrawEngine::RED, 0, true)))
			err = FAILURE;
		if (FAILED(pDrawEngine->WriteText(screenWidth / 2, screenHeight / 2 , bottomText.c_str(), MyDrawEngine::YELLOW, 0, true)))
			err = FAILURE;
		if (FAILED(pDrawEngine->WriteText(screenWidth / 2, screenHeight / 2 + 80, continueText.c_str(), MyDrawEngine::RED, 0, true)))
			err = FAILURE;

		if (FAILED(err))
		{
			ErrorLogger::Writeln(L"Draw error in DungeonGameManager::Render().");
		}
	}
	else if (mGameState == GameState::victory)
	{
		ErrorType err = SUCCESS;
		const std::wstring victoryText = L"You have succeeded. You prevailed over your foes and conquered the Dungeon.";
		const std::wstring victoryText2 = L"Now you can go conquer it again - but faster!";
		const std::wstring bottomText = L"You conquered all " + std::to_wstring(mNumLevels) + L" levels in " + std::to_wstring(mTimer) + L" seconds."; // don't trim this time
		const std::wstring continueText = L"Press [Spacebar] to play again. Press [Escape] to return to menu.";

		const int screenWidth = pDrawEngine->GetScreenWidth();
		const int screenHeight = pDrawEngine->GetScreenHeight();

		if (FAILED(pDrawEngine->WriteText(screenWidth / 2, screenHeight / 2 - 90, victoryText.c_str(), MyDrawEngine::GREEN, 0, true)))
			err = FAILURE;
		if (FAILED(pDrawEngine->WriteText(screenWidth / 2, screenHeight / 2 - 65, victoryText2.c_str(), MyDrawEngine::GREEN, 0, true)))
			err = FAILURE;
		if (FAILED(pDrawEngine->WriteText(screenWidth / 2, screenHeight / 2, bottomText.c_str(), MyDrawEngine::YELLOW, 0, true)))
			err = FAILURE;
		if (FAILED(pDrawEngine->WriteText(screenWidth / 2, screenHeight / 2 + 80, continueText.c_str(), MyDrawEngine::RED, 0, true)))
			err = FAILURE;

		if (FAILED(err))
		{
			ErrorLogger::Writeln(L"Draw error in DungeonGameManager::Render().");
		}
	}
}

bool DungeonGameManager::ContinuePressed()
{
	return mpServiceManager.lock()->GetInputs().lock()->NewKeyPressed(DIK_SPACE);
}

bool DungeonGameManager::HandleBoonSelection()
{
	if (BoonSelected())
		return true;

	const std::shared_ptr<MyInputs> pInputs = mpServiceManager.lock()->GetInputs().lock();

	std::unique_ptr<Boon> selectedBoon;
	if (pInputs->NewKeyPressed(DIK_1))
		selectedBoon = std::make_unique<HealthBoon>(mpPlayerKnight, 3);
	else if (pInputs->NewKeyPressed(DIK_2))
		selectedBoon = std::make_unique<DamageBoon>(mpPlayerKnight, 1);
	else if (pInputs->NewKeyPressed(DIK_3))
		selectedBoon = std::make_unique<SpeedBoon>(mpPlayerKnight, 2.f);

	if (selectedBoon)
	{
		mSelectedBoons.push_back(std::move(selectedBoon));

		const std::shared_ptr<SFXManager> pSFX = mpServiceManager.lock()->GetSFXManager().lock();
		if (typeid(*pSFX) == typeid(DungeonSFXManager))
		{
			std::shared_ptr<DungeonSFXManager> pSFXManager = std::static_pointer_cast<DungeonSFXManager>(pSFX);
			if (pSFXManager)
			{
				pSFXManager->PlayBoonAcquired();
			}
		}

		return true;
	}
	else
	{
		return false;
	}
}

bool DungeonGameManager::BoonSelected()
{
	return mSelectedBoons.size() == mActiveLevelId;
}

void DungeonGameManager::ApplyBoons(const std::weak_ptr<Knight>& pPlayerKnight)
{
	for (const std::unique_ptr<Boon>& boon : mSelectedBoons)
	{
		boon->SetKnight(pPlayerKnight);
		boon->Apply();
	}
}
