#include "ServiceManager.h"
#include <winerror.h>
#include "ErrorLogger.h"

#include "mydrawengine.h"
#include "ObjectManager.h"
#include "SFXManager.h"
#include "MyInputs.h"


// PUBLIC

ServiceManager::ServiceManager()
{
}

ServiceManager::~ServiceManager()
{
}

// Create the engines - this should be done before creating other DDraw objects
ErrorType ServiceManager::StartServices(bool fullScreen, HWND hwnd, HINSTANCE hinstance)
{
	mpDrawEngine = std::make_shared<MyDrawEngine>(hwnd);
	if (!mpDrawEngine || FAILED(mpDrawEngine->Start(fullScreen)))
	{
		ErrorLogger::Writeln(L"Failed to start MyDrawEngine");
		return FAILURE;
	}

	mpInputs = std::make_shared<MyInputs>(hinstance, hwnd);
	if (!mpInputs)
	{
		ErrorLogger::Writeln(L"Failed to start MyInputs");
		return FAILURE;
	}

	mpObjectManager = std::make_shared<ObjectManager>();
	if (!mpObjectManager)
	{
		ErrorLogger::Writeln(L"Failed to create ObjectManager");
		return FAILURE;
	}

	// Child must create an instance of a GameObjectFactory subclass.
	// Child must create an instance of a SFXManager subclass.

	return SUCCESS;
}

std::weak_ptr<ObjectManager> ServiceManager::GetObjectManager()
{
	return mpObjectManager;
}

std::weak_ptr<GameObjectFactory> ServiceManager::GetObjectFactory()
{
	return mpObjectFactory;
}

std::weak_ptr<MyDrawEngine> ServiceManager::GetDrawEngine()
{
	return mpDrawEngine;
}

std::weak_ptr<SFXManager> ServiceManager::GetSFXManager()
{
	return mpSfxManager;
}

std::weak_ptr<MyInputs> ServiceManager::GetInputs()
{
	return mpInputs;
}
