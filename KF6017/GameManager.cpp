/*
* GameManager.cpp
* 19002175
*
* Abstract class for a service managing levels and objectives.
* Intended to be inherited from by a game-specific class.
*/

#include "GameManager.h"


// PUBLIC

GameManager::GameManager(std::shared_ptr<ServiceManager> pServiceManager)
	: Super(), mpServiceManager(pServiceManager)
{
}

GameManager::~GameManager()
{
}

void GameManager::Update(double deltaTime)
{
	Render();
}


// PROTECTED

void GameManager::Render()
{
}
