#include "ServiceManager.h"
#include <winerror.h>
#include "ErrorLogger.h"

#include "mydrawengine.h"
#include "ObjectManager.h"
#include "GameObjectFactory.h"
#include "MySoundEngine.h"
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

	soundEngine = std::make_shared<MySoundEngine>(hwnd);
	if (!soundEngine)
	{
		ErrorLogger::Writeln(L"Failed to start MySoundEngine");
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

std::weak_ptr<MySoundEngine> ServiceManager::GetSoundEngine()
{
	return soundEngine;
}

std::weak_ptr<MyInputs> ServiceManager::GetInputs()
{
	return inputs;
}
