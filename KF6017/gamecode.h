#pragma once
#include <memory>
#include "errortype.h"
#include "windows.h"
#include "gametimer.h"

class ServiceManager;
class Menu;

// This is a hack for reading keyboard in situations where you don't want to
// use a full input system.
// Note that this makes the class windows-specific, but it already includes
// windows.h.
// Still - nasty.
#define KEYPRESSED(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

// The game class. (Singleton)
// Manages a game loop and application state (paused, running, exited, ...)
class Game
{
public:
	static Game instance;          // Singleton instance

   // Starts the game engines and services, intialises menus
	// This is called soon after the program runs
	ErrorType Setup(bool bFullScreen, HWND hwnd, HINSTANCE hinstance);

	// This is repeated, called every frame.
	// Flips and clears the back buffer.
	// Decides whether to tun game or menus depending on the game state
	ErrorType Main();

	// Called at the start of the game - when changing state from MENU to RUNNING
	// Initialises game.
	ErrorType StartOfGame();

	// Called each frame when in the RUNNING state.
	ErrorType GameUpdate();

	// Called from Update() to handle game object agnostic player input.
	// Checks for user pressing escape (which puts the game in the PAUSED state)
	ErrorType HandleInput();

	// Called when the player ends the game.
	// Currently this is done from the PAUSED state, when returning to the main menu.
	ErrorType EndOfGame();

	// This is called just before the program exits
	void Shutdown();

private:
	Game();
	Game(Game& other); // Copy constructor disabled
	~Game();

	enum class GameState { menu, paused, running, gameOver };
	GameState mCurrentState;      // Current state of the game 
								  // Menu = start menu
								  // Paused = paused
								  // Running = the main game loop
								  // Gameover = setting this state causes the program to close
								  //            after tidying up
	
	// Changes the state of the game to one of the states above
	void ChangeState(GameState newState);  

	GameTimer mGameTimer;
	std::shared_ptr<ServiceManager> mpServiceManager;

	std::unique_ptr<Menu> mMainMenu;
	std::unique_ptr<Menu> mPauseMenu;
};




