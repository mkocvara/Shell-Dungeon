#include "GameManager.h"


// PUBLIC

GameManager::GameManager(std::shared_ptr<ServiceManager> pServiceManager)
	: mpServiceManager(pServiceManager)
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
