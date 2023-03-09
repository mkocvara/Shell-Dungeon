#include "AsteroidsServiceManager.h"
#include "ErrorLogger.h"
#include "AsteroidsObjectFactory.h"

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

	return SUCCESS;
}