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

Game::Game()
{
}

Game::~Game()
{
}

Game Game::instance;    // Singleton instance

// Starts the game engines - Draw Engine, Sound Engine, Input Engine - singletons
// This is called soon after the program runs
ErrorType Game::Setup(bool bFullScreen, HWND hwnd, HINSTANCE hinstance)
{
	mpServiceManager = std::make_shared<AsteroidsServiceManager>();
	return mpServiceManager->StartServices(bFullScreen, hwnd, hinstance);
}

// This is repeated, called every frame.
// It will run either Update( ), MainMenu() or PauseMenu() depending on the
// game state
ErrorType Game::Main()
{
	std::shared_ptr<MyDrawEngine> pDrawEngine = mpServiceManager->GetDrawEngine().lock();
	if (!pDrawEngine)
		return FAILURE;

	//Flip and clear the back buffer
	pDrawEngine->Flip();
	pDrawEngine->ClearBackBuffer();

	ErrorType err=SUCCESS;

	switch(m_currentState)
	{
	case MENU:

		err= MainMenu();     // Menu at start of game
		break;
	case PAUSED:
		err = PauseMenu();   // Player has paused the game
		break;
	case RUNNING:           // Playing the actual game
		err= Update();
		break;
   case GAMEOVER:
		err = FAILURE;       // Error return causes the window loop to exit
	default:
		// Not a valid state
		err = FAILURE;       // Error return causes the window loop to exit
	}

	return err;
}

// Use to change the state of the game. Private function
void Game::ChangeState(GameState newState)
{
	// Very crude state system
	// Close old state
	switch(m_currentState)
	{
	case MENU:
      // Not needed
		break;
	case PAUSED:
      // Not needed
		break;
	case RUNNING:
      // Not needed
		break;
	}

	// Change the state
	m_currentState = newState;
	m_menuOption = 0;

	// Transition to new state
	switch(m_currentState)
	{
	case MENU:
      // Not needed
		break;
	case PAUSED:
      // Not needed
		break;
	case RUNNING:
      // Not needed
		break;
	}
}

// Terminates the game engines - Draw Engine, Sound Engine, Input Engine
// This is called just before the program exits
void Game::Shutdown()
{
	// Any clean up code here 


	/*/ DO NOT TERMINATE - unnecessary with smart pointers
	// (engines must be terminated last)
	MyDrawEngine::Terminate();
	MySoundEngine::Terminate();
	MyInputs::Terminate();
	/*/
}


// **********************************************************************************************
// Placeholder menus  ***************************************************************************
// **********************************************************************************************

// Called each frame when in the pause state. Manages the pause menu
// which is currently a basic placeholder
ErrorType Game::PauseMenu()
{
	// Code for a basic pause menu
	std::shared_ptr<MyInputs> pInputs = mpServiceManager->GetInputs().lock();
	std::shared_ptr<MyDrawEngine> pDrawEngine = mpServiceManager->GetDrawEngine().lock();

	if (!pDrawEngine || !pInputs)
		return FAILURE;

	pDrawEngine->WriteText(450,220, L"Paused", MyDrawEngine::WHITE);

	const int NUMOPTIONS = 2;
	wchar_t options[NUMOPTIONS][11] = {L"Resume", L"Main menu"};

   // Display menu options
	for(int i=0;i<NUMOPTIONS;i++)
	{
		int colour = MyDrawEngine::GREY;       // If not selected, should be grey
		if(i == m_menuOption)
		{
			colour = MyDrawEngine::WHITE;       // Current selection is white
		}
		pDrawEngine->WriteText(450,300+50*i, options[i], colour);
	}

   // Get user input
	pInputs->SampleKeyboard();

   // Move choice up and down
	if(pInputs->NewKeyPressed(DIK_UP))
	{
		m_menuOption--;
	}
	if(pInputs->NewKeyPressed(DIK_DOWN))
	{
		m_menuOption++;
	}
	if(m_menuOption<0)
	{
		m_menuOption=0;
	}
	else if(m_menuOption>=NUMOPTIONS)
	{
		m_menuOption=NUMOPTIONS-1;
	}

   // If player chooses an option ....
	if(pInputs->NewKeyPressed(DIK_RETURN))
	{
		if(m_menuOption ==0)      // Resume
		{
			ChangeState(RUNNING);  // Go back to running the game
		}
		if(m_menuOption ==1)      // Quit
		{
			EndOfGame();           // Clear up the game
			ChangeState(MENU);     // Go back to the menu
		}
	}

	return SUCCESS;
}

// Called each frame when in the menu state. Manages the menu
// which is currently a basic placeholder
ErrorType Game::MainMenu()
{
	std::shared_ptr<MyInputs> pInputs = mpServiceManager->GetInputs().lock();
	std::shared_ptr<MyDrawEngine> pDrawEngine = mpServiceManager->GetDrawEngine().lock();

	if (!pDrawEngine || !pInputs)
		return FAILURE;

	pDrawEngine->WriteText(450,220, L"Main menu", MyDrawEngine::WHITE);

	const int NUMOPTIONS = 2;
	wchar_t options[NUMOPTIONS][15] = {L"Start game", L"Exit"};

   // Display the options
	for(int i=0;i<NUMOPTIONS;i++)
	{
		int colour = MyDrawEngine::GREY;
		if(i == m_menuOption)
		{
			colour = MyDrawEngine::WHITE;
		}
		pDrawEngine->WriteText(450,300+50*i, options[i], colour);
	}

   // Get keyboard input
	pInputs->SampleKeyboard();
	if(pInputs->NewKeyPressed(DIK_UP))
	{
		m_menuOption--;
	}
	if(pInputs->NewKeyPressed(DIK_DOWN))
	{
		m_menuOption++;
	}
	if(m_menuOption<0)
	{
		m_menuOption=0;
	}
	else if(m_menuOption>=NUMOPTIONS)
	{
		m_menuOption=NUMOPTIONS-1;
	}

   // User selects an option
	if(pInputs->NewKeyPressed(DIK_RETURN))
	{
		if(m_menuOption ==0)          // Play
		{  
			StartOfGame();             // Initialise the game
			ChangeState(RUNNING);      // Run it
		}

		if(m_menuOption ==1)          //Quit
		{
			ChangeState(GAMEOVER);
		}
	}

	return SUCCESS;
}


// **********************************************************************************************
// The game !!! *********************************************************************************
// **********************************************************************************************

// Called at the start of the game - when changing state from MENU to RUNNING
// Use this to initialise the core game
ErrorType Game::StartOfGame()
{
	std::shared_ptr<ObjectManager> pObjectManager = mpServiceManager->GetObjectManager().lock();
	if (!pObjectManager)
		return FAILURE;

	std::shared_ptr<GameObjectFactory> pObjectFactory = mpServiceManager->GetObjectFactory().lock();
	if (!pObjectFactory)
		return FAILURE;

	pObjectFactory->Create(ObjectType::spaceship, mpServiceManager);

	for (int i = 0; i < 6; i++)
	{
		pObjectFactory->Create(ObjectType::spacerock, mpServiceManager);
	}

	m_gameTimer.begin();

	return SUCCESS;
}


// Called each frame when in the RUNNING state.
// Checks for user pressing escape (which puts the game in the PAUSED state)
// Flips and clears the back buffer
// Gameplay programmer will develop this to create an actual game
ErrorType Game::Update()
{
	m_gameTimer.mark();

	if(FAILED(HandleInput()))
		return FAILURE;

	std::shared_ptr<ObjectManager> pObjectManager = mpServiceManager->GetObjectManager().lock();

	pObjectManager->UpdateAll(m_gameTimer.mFrameTime);

#if _DEBUG
	std::shared_ptr<MyDrawEngine> pDrawEngine = mpServiceManager->GetDrawEngine().lock();
	if (!pDrawEngine)
		return FAILURE;

	pDrawEngine->WriteInt(50, 50, pObjectManager->GetNumberOfObjects(), MyDrawEngine::GREEN);
#endif

	return SUCCESS;
}

ErrorType Game::HandleInput()
{
	// Check for entry to pause menu
	if (KEYPRESSED(VK_ESCAPE))
	{
		ChangeState(PAUSED);
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

