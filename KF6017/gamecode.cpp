#include "gamecode.h"
#include "mydrawengine.h"
#include "mysoundengine.h"
#include "myinputs.h"
#include <time.h>
#include "gametimer.h"
#include "errorlogger.h"
#include <math.h>
#include "shapes.h"
#include "Spaceship.h"
#include "RandUtil.h"
#include "SpaceRock.h"
#include "AsteroidsServiceManager.h"
#include "ObjectManager.h"
#include "GameObjectFactory.h"
#include "GameManager.h"
#include "Menu.h"

#include "ErrorLogger.h"

Game::Game()
{
}

Game::~Game()
{
}

Game Game::instance;    // Singleton instance

// Starts the game engines and services, intialises menus
// This is called soon after the program runs
ErrorType Game::Setup(bool bFullScreen, HWND hwnd, HINSTANCE hinstance)
{
	// Start services
	mpServiceManager = AsteroidsServiceManager::Create();
	ErrorType err = mpServiceManager->StartServices(bFullScreen, hwnd, hinstance);

	// Setup menus
	mMainMenu = std::make_unique<Menu>(mpServiceManager, L"Main Menu");
	mMainMenu->AddItem(L"Start Game", [this]() 
		{ 
			StartOfGame(); 
			ChangeState(GameState::running);
		});
	mMainMenu->AddItem(L"Exit", [this]() 
		{
			ChangeState(GameState::gameOver);
		});
	
	mPauseMenu = std::make_unique<Menu>(mpServiceManager, L"Paused");
	mPauseMenu->AddItem(L"Resume", [this]() 
		{ 
			ChangeState(GameState::running);
		});
	mPauseMenu->AddItem(L"Restart Level", [this]() 
		{
			std::shared_ptr<GameManager> pGameManager = mpServiceManager->GetGameManager().lock();
			pGameManager->RestartLevel();	
			ChangeState(GameState::running);
		});
	mPauseMenu->AddItem(L"Main Menu", [this]() 
		{ 
			EndOfGame(); 
			ChangeState(GameState::menu);
		});

	return err;
}

// This is repeated, called every frame.
// Flips and clears the back buffer
// It will run either Update(), MainMenu() or PauseMenu() depending on the game state
ErrorType Game::Main()
{
	std::shared_ptr<MyDrawEngine> pDrawEngine = mpServiceManager->GetDrawEngine().lock();
	if (!pDrawEngine)
		return FAILURE;

	//Flip and clear the back buffer
	pDrawEngine->Flip();
	pDrawEngine->ClearBackBuffer();

	ErrorType err=SUCCESS;

	mGameTimer.mark();

	switch(mCurrentState)
	{
	case GameState::menu:

		err = mMainMenu->Update();  // Menu at start of game
		break;
	case GameState::paused:
		err = mPauseMenu->Update(); // Player has paused the game
		break;
	case GameState::running:					// Playing the actual game
		err = Update();
		break;
   case GameState::gameOver:
		err = FAILURE;				// Error return causes the window loop to exit
	default:
		// Not a valid state
		err = FAILURE;				// Error return causes the window loop to exit
	}

	return err;
}

// Use to change the state of the game. Private function
void Game::ChangeState(GameState newState)
{
	// Very crude state system
	// Close old state
	switch(mCurrentState)
	{
	case GameState::menu:
      // Not needed
		break;
	case GameState::paused:
      // Not needed
		break;
	case GameState::running:
      // Not needed
		break;
	}

	// Change the state
	mCurrentState = newState;

	// Transition to new state
	switch(mCurrentState)
	{
	case GameState::menu:
      // Not needed
		break;
	case GameState::paused:
      // Not needed
		break;
	case GameState::running:
      // Not needed
		break;
	}
}

// This is called just before the program exits
void Game::Shutdown()
{
	// Any clean up code here 
}

// **********************************************************************************************
// The game !!! *********************************************************************************
// **********************************************************************************************

// Called at the start of the game - when changing state from MENU to RUNNING
// Use this to initialise the core game
ErrorType Game::StartOfGame()
{
	std::shared_ptr<GameManager> pGameManager = mpServiceManager->GetGameManager().lock();
	pGameManager->StartLevel(1);

	mGameTimer.begin();

	return SUCCESS;
}


// Called each frame when in the RUNNING state.
// Checks for user pressing escape (which puts the game in the PAUSED state)
// Gameplay programmer will develop this to create an actual game
ErrorType Game::Update()
{
	if(FAILED(HandleInput()))
		return FAILURE;

	std::shared_ptr<ObjectManager> pObjectManager = mpServiceManager->GetObjectManager().lock();
	std::shared_ptr<GameManager> pGameManager = mpServiceManager->GetGameManager().lock();

	pObjectManager->UpdateAll(mGameTimer.mFrameTime);
	pGameManager->Update(mGameTimer.mFrameTime);

	return SUCCESS;
}

ErrorType Game::HandleInput()
{
	// Check for entry to pause menu
	if (KEYPRESSED(VK_ESCAPE))
	{
		ChangeState(GameState::paused);
	}

	return SUCCESS;
}

// Called when the player ends the game
// Currently this is done from the PAUSED state, when returning to the main menu
// but could be done by the gameplay programmer in other situations
// This will be used by the gameplay programmer to clean up
ErrorType Game::EndOfGame()
// called when the game ends by returning to main menu
{
	std::shared_ptr<ObjectManager> pObjectManager = mpServiceManager->GetObjectManager().lock();
	if (!pObjectManager)
		return FAILURE;

	pObjectManager->Clear();

	return SUCCESS;
}

