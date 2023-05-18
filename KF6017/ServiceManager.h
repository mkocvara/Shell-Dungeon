/*
* ServiceManager.h
* 19002175
*
* Abstract base ServiceManager. Keeps a reference to all services and IO engines. 
* Used to access all services by game objects via dependency injection.
* Intended to be inherited from by a game-specific subclass.
*/

#pragma once
#include <memory>
#include <list>
#include "errortype.h"
#include "windows.h"

class Service;
class MyDrawEngine;
class MyInputs;
class SFXManager;
class ObjectManager;
class GameObjectFactory;
class EventManager;
class GameManager;
class MousePointer;

class ServiceManager : public std::enable_shared_from_this<ServiceManager>
{
public:
	virtual ~ServiceManager();

	virtual ErrorType StartServices(bool bFullScreen, HWND hwnd, HINSTANCE hinstance);
	std::shared_ptr<ServiceManager> GetSharedPtrFromThis();

	std::weak_ptr<MyDrawEngine> GetDrawEngine();
	std::weak_ptr<MyInputs> GetInputs();

	std::weak_ptr<ObjectManager> GetObjectManager();
	std::weak_ptr<EventManager> GetEventManager();
	std::weak_ptr<MousePointer> GetMousePointer();

	// GameObjectFactory, SFXManager, GameManager are abstract; 
	// they must be instantiated by a subclass!
	std::weak_ptr<SFXManager> GetSFXManager();
	std::weak_ptr<GameObjectFactory> GetObjectFactory();
	std::weak_ptr<GameManager> GetGameManager();

	// Mind that this returns a copy of the list with new shared ptrs, sharing ownership!
	std::list<std::shared_ptr<Service>> GetAllServices();

protected:
	ServiceManager(); // abstract

	std::list<std::shared_ptr<Service>> mAllServices;

	std::shared_ptr<MyDrawEngine> mpDrawEngine;
	std::shared_ptr<MyInputs> mpInputs;

	std::shared_ptr<ObjectManager> mpObjectManager;
	std::shared_ptr<EventManager> mpEventManager;
	std::shared_ptr<MousePointer> mpMousePointer;
	std::shared_ptr<SFXManager> mpSfxManager;
	std::shared_ptr<GameObjectFactory> mpObjectFactory;
	std::shared_ptr<GameManager> mpGameManager;
};

