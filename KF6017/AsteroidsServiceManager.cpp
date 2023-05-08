#include "AsteroidsServiceManager.h"
#include "ErrorLogger.h"
#include "AsteroidsObjectFactory.h"
#include "AsteroidsSFXManager.h"
#include "AsteroidsGameManager.h"

#include "MousePointer.h"

AsteroidsServiceManager::AsteroidsServiceManager() : Super()
{
}


// PUBLIC

AsteroidsServiceManager::~AsteroidsServiceManager()
{
}

std::shared_ptr<AsteroidsServiceManager> AsteroidsServiceManager::Create()
{
	return std::shared_ptr<AsteroidsServiceManager>(new AsteroidsServiceManager());
}

ErrorType AsteroidsServiceManager::StartServices(bool fullScreen, HWND hwnd, HINSTANCE hinstance)
{
	if (FAILED(Super::StartServices(fullScreen, hwnd, hinstance)))
		return FAILURE;

	// Services
	mpSfxManager = std::make_shared<AsteroidsSFXManager>(hwnd);
	if (!mpSfxManager)
	{
		ErrorLogger::Writeln(L"Failed to create AsteroidsSFXManager");
		return FAILURE;
	}
	mAllServices.push_back(mpSfxManager);

	mpObjectFactory = std::make_shared<AsteroidsObjectFactory>();
	if (!mpObjectFactory)
	{
		ErrorLogger::Writeln(L"Failed to create AsteroidsObjectFactory");
		return FAILURE;
	}
	mAllServices.push_back(mpObjectFactory);

	mpGameManager = std::make_shared<AsteroidsGameManager>(std::move(GetSharedPtrFromThis()));
	if (!mpGameManager)
	{
		ErrorLogger::Writeln(L"Failed to create AsteroidsGameManager");
		return FAILURE;
	}
	mAllServices.push_back(mpGameManager);

	mpMousePointer->SetPointerEnabled(false);

	return SUCCESS;
}