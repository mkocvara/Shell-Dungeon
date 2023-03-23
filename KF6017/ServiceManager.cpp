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
ErrorType ServiceManager::StartServices(bool bFullScreen, HWND hwnd, HINSTANCE hinstance)
{
	drawEngine = std::make_shared<MyDrawEngine>(hwnd);
	if (!drawEngine || FAILED(drawEngine->Start(bFullScreen)))
	{
		ErrorLogger::Writeln(L"Failed to start MyDrawEngine");
		return FAILURE;
	}

	inputs = std::make_shared<MyInputs>(hinstance, hwnd);
	if (!inputs)
	{
		ErrorLogger::Writeln(L"Failed to start MyInputs");
		return FAILURE;
	}

	objectManager = std::make_shared<ObjectManager>();
	if (!objectManager)
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
	return objectManager;
}

std::weak_ptr<GameObjectFactory> ServiceManager::GetObjectFactory()
{
	return objectFactory;
}

std::weak_ptr<MyDrawEngine> ServiceManager::GetDrawEngine()
{
	return drawEngine;
}

std::weak_ptr<SFXManager> ServiceManager::GetSFXManager()
{
	return sfxManager;
}

std::weak_ptr<MyInputs> ServiceManager::GetInputs()
{
	return inputs;
}
