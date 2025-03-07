#pragma once
#include <list>
#include <map>
#include <memory>
#include <typeindex>
#include "Event.h"

class IEventCallback 
{
private:
	virtual void Call(Event& e) = 0;

public:
	virtual ~IEventCallback() = default;

	void Execute(Event& e) {
		Call(e);
	}
};

template <typename TOwner, typename TEvent>
class EventCallback : public IEventCallback
{
private:
	typedef void(TOwner::*CallbackFunction)(TEvent&);

	TOwner* OwnerInstance;
	CallbackFunction CallbackFunction_;

	virtual void Call(Event& e) override {
		std::invoke(CallbackFunction_, OwnerInstance, static_cast<TEvent&>(e));
	}

public:
	EventCallback(TOwner* ownerInstance, CallbackFunction callbackFunction) {
		this->OwnerInstance = ownerInstance;
		this->CallbackFunction_ = callbackFunction;
	}

	virtual ~EventCallback() override = default;
};

typedef std::list<std::unique_ptr<IEventCallback>> HandlerList;

class EventBus 
{
private:
	// Type of event/ List of subscribed events
	std::map<std::type_index, std::unique_ptr<HandlerList>> Subscribers;
public:
	EventBus();
	~EventBus();

	// Clear the subscriber list
	void Reset();

	/////////////////////////////////////////////////////////////////////////////
	// Subscribe to an event of type <T>
	// a listener subscribes to an event
	// Example: eventBus->SubscribeToEvent<CollisionEvent>(this, &Game::OnCollision);
	/////////////////////////////////////////////////////////////////////////////
	template <typename TEvent, typename TOwner>
	void SubscribeToEvent(TOwner* ownerInstance, void (TOwner::*callbackFunction)(TEvent&)) {
		if (Subscribers[typeid(TEvent)].get() == nullptr)
		{
			Subscribers[typeid(TEvent)] = std::make_unique<HandlerList>();
		}
		auto subscriber = std::make_unique<EventCallback<TOwner, TEvent>>(ownerInstance, callbackFunction);
		Subscribers[typeid(TEvent)]->push_back(std::move(subscriber));
	};

	/////////////////////////////////////////////////////////////////////////////
	// Emit an event of type <T>
	// as soon as something emits an event we execute all the listener callback
	// functions
	// Example: eventBus->EmitEvent<CollisionEvent>(player, enemy);
	/////////////////////////////////////////////////////////////////////////////
	template <typename TEvent, typename ...TArgs>
	void EmitEvent(TArgs&& ...args) {
		auto handlers = Subscribers[typeid(TEvent)].get();
		if (handlers) 
		{
			for (auto it = handlers->begin(); it != handlers->end(); it++)
			{
				auto handler = it->get();
				TEvent event(std::forward<TArgs>(args)...);
				handler->Execute(event);
			}
		}
	};
};
