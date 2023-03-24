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

ErrorType AsteroidsServiceManager::StartServices(bool fullScreen, HWND hwnd, HINSTANCE hinstance)
{
	if (FAILED(Super::StartServices(fullScreen, hwnd, hinstance)))
		return FAILURE;

	mpObjectFactory = std::make_shared<AsteroidsObjectFactory>();
	if (!mpObjectFactory)
	{
		ErrorLogger::Writeln(L"Failed to create AsteroidsObjectFactory");
		return FAILURE;
	}

	mpSfxManager = std::make_shared<AsteroidsSFXManager>(hwnd);
	if (!mpSfxManager)
	{
		ErrorLogger::Writeln(L"Failed to create AsteroidsSFXManager");
		return FAILURE;
	}

	return SUCCESS;
}