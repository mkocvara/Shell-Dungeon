/*
* DungeonServiceManager.cpp
* 19002175
*
* Game-specific ServiceManager. Keeps a reference to all services and IO engines.
* Used to access all services by game objects via dependency injection.
* Creates game-specific services.
*/

#include "DungeonServiceManager.h"
#include "ErrorLogger.h"
#include "DungeonObjectFactory.h"
#include "DungeonSFXManager.h"
#include "DungeonGameManager.h"

DungeonServiceManager::DungeonServiceManager() : Super()
{
}


// PUBLIC

DungeonServiceManager::~DungeonServiceManager()
{
}

std::shared_ptr<DungeonServiceManager> DungeonServiceManager::Create()
{
	return std::shared_ptr<DungeonServiceManager>(new DungeonServiceManager());
}

ErrorType DungeonServiceManager::StartServices(bool fullScreen, HWND hwnd, HINSTANCE hinstance)
{
	if (FAILED(Super::StartServices(fullScreen, hwnd, hinstance)))
		return FAILURE;

	// Services
	mpSfxManager = std::make_shared<DungeonSFXManager>(hwnd);
	if (!mpSfxManager)
	{
		ErrorLogger::Writeln(L"Failed to create DungeonSFXManager");
		return FAILURE;
	}
	mAllServices.push_back(mpSfxManager);

	mpObjectFactory = std::make_shared<DungeonObjectFactory>(std::move(GetSharedPtrFromThis()));
	if (!mpObjectFactory)
	{
		ErrorLogger::Writeln(L"Failed to create DungeonObjectFactory");
		return FAILURE;
	}
	mAllServices.push_back(mpObjectFactory);

	mpGameManager = std::make_shared<DungeonGameManager>(std::move(GetSharedPtrFromThis()));
	if (!mpGameManager)
	{
		ErrorLogger::Writeln(L"Failed to create DungeonGameManager");
		return FAILURE;
	}
	mAllServices.push_back(mpGameManager);

	return SUCCESS;
}