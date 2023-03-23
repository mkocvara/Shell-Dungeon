#include "AsteroidsServiceManager.h"
#include "ErrorLogger.h"
#include "AsteroidsObjectFactory.h"
#include "AsteroidsSFXManager.h"

AsteroidsServiceManager::AsteroidsServiceManager() : Super()
{
}

AsteroidsServiceManager::~AsteroidsServiceManager()
{
}

ErrorType AsteroidsServiceManager::StartServices(bool bFullScreen, HWND hwnd, HINSTANCE hinstance)
{
	if (FAILED(Super::StartServices(bFullScreen, hwnd, hinstance)))
		return FAILURE;

	objectFactory = std::make_shared<AsteroidsObjectFactory>();
	if (!objectFactory)
	{
		ErrorLogger::Writeln(L"Failed to create AsteroidsObjectFactory");
		return FAILURE;
	}

	sfxManager = std::make_shared<AsteroidsSFXManager>(hwnd);
	if (!sfxManager)
	{
		ErrorLogger::Writeln(L"Failed to create AsteroidsSFXManager");
		return FAILURE;
	}

	return SUCCESS;
}