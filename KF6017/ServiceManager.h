#pragma once
#include <memory>
#include "errortype.h"
#include "windows.h"

class ObjectManager;
class GameObjectFactory;
class MyDrawEngine;
class SFXManager;
class MyInputs;
class EventManager;

class ServiceManager
{
public:
	virtual ~ServiceManager();

	virtual ErrorType StartServices(bool bFullScreen, HWND hwnd, HINSTANCE hinstance);

	std::weak_ptr<ObjectManager> GetObjectManager();
	std::weak_ptr<MyDrawEngine> GetDrawEngine();
	std::weak_ptr<MyInputs> GetInputs();
	std::weak_ptr<EventManager> GetEventManager();

	// GameObjectFactory and SFXManager are abstract; they must be instantiated by a subclass
	std::weak_ptr<GameObjectFactory> GetObjectFactory();
	std::weak_ptr<SFXManager> GetSFXManager();

protected:
	ServiceManager(); // abstract

	std::shared_ptr<ObjectManager> mpObjectManager;
	std::shared_ptr<GameObjectFactory> mpObjectFactory;
	std::shared_ptr<MyDrawEngine> mpDrawEngine;
	std::shared_ptr<SFXManager> mpSfxManager;
	std::shared_ptr<MyInputs> mpInputs;
	std::shared_ptr<EventManager> mpEventManager;
};

