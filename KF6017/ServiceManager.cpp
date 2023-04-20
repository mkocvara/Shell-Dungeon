#include "ServiceManager.h"
#include <winerror.h>
#include "ErrorLogger.h"

#include "mydrawengine.h"
#include "ObjectManager.h"
#include "SFXManager.h"
#include "MyInputs.h"
#include "EventManager.h"
#include "GameManager.h"


ServiceManager::ServiceManager()
{
}


// PUBLIC

ServiceManager::~ServiceManager()
{
}

// Create the engines - this should be done before creating other DDraw objects
ErrorType ServiceManager::StartServices(bool fullScreen, HWND hwnd, HINSTANCE hinstance)
{
	// IO engines
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

	// Services
	mpEventManager = std::make_shared<EventManager>(std::move(GetSharedPtrFromThis()));
	if (!mpEventManager)
	{
		ErrorLogger::Writeln(L"Failed to create EventManager");
		return FAILURE;
	}
	mAllServices.push_back(mpEventManager);

	// Child must create an instance of a GameObjectFactory subclass.
	// Child must create an instance of a SFXManager subclass.
	// Child must create an instance of a GameManager subclass.

	return SUCCESS;
}

std::shared_ptr<ServiceManager> ServiceManager::GetSharedPtrFromThis()
{
	return shared_from_this();
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

std::weak_ptr<EventManager> ServiceManager::GetEventManager()
{
	return mpEventManager;
}

std::weak_ptr<GameManager> ServiceManager::GetGameManager()
{
	return mpGameManager;
}

std::list<std::shared_ptr<Service>> ServiceManager::GetAllServices()
{
	return mAllServices;
}
