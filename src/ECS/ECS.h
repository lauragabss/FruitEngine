#pragma once
#include <bitset>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <set>
#include <memory>
#include "../Logger/Logger.h"

using std::shared_ptr;

const unsigned int MAX_COMPONENTS = 32;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Signature
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Use a bitset to keep track of which components an entity has,
// also helps keep track of which entities a system is interested in.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef std::bitset<MAX_COMPONENTS> Signature;

struct IComponent 
{
protected:
	static int nextId;
};

// base of all components used in the engine
// Used to assign a unique id to a component type
template <typename T>
class Component : public IComponent
{
public:
	// retunrs the unique id of component <T>
	static int GetId() {
		static auto id = nextId++;
		return id;
	}

};

// Equivalent to an actor in unreal engine
class Entity 
{
private:
	int id;

public:
	Entity(int id) : id(id) {};
	int GetId() const { return id; };

	class Registry* Registry_ = nullptr;

	// Component Management -> Calls Registry's actual code
	template <typename TComponent, typename ...TArgs> void AddComponent(TArgs&& ...args);
	template <typename TComponent> void RemoveComponent();
	template <typename TComponent> bool HasComponent() const;
	template <typename TComponent> TComponent& GetComponent() const;

	// overload the == operator
	bool operator == (const Entity& other) const { return GetId() == other.GetId(); };
	bool operator != (const Entity& other) const { return GetId() != other.GetId(); };
	bool operator > (const Entity& other) const { return GetId() > other.GetId(); };
	bool operator < (const Entity& other) const { return GetId() < other.GetId(); };
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// System
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The system processes entities that contain a specific signature
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class System 
{
private:
	Signature ComponentSignature;
	std::vector<Entity> Entities;

public:
	System () = default;
	~System () = default;

	void AddEntityToSystem(Entity entity);
	void RemoveEntityFromSystem(Entity entity);
	std::vector<Entity> GetSystemEntities() const;
	const Signature& GetComponentSignature() const;

	// Defines the component types the entities must have to be considered by the system
	template <typename TComponent> void RequireComponent();
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Pool
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A pool is just a vector of objects of type T
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class IPool {
public:
	// TO force the class to be only abstract
	virtual ~IPool() {};
};

template <typename T>
class Pool  : public IPool
{
private:
	std::vector<T> Data;

public:
	Pool(int size = 100) {
		Data.resize(size);
	};

	virtual ~Pool() = default;

	bool isEmpty() const {
		return Data.empty();
	};

	int GetSize() const {
		return Data.size();
	};

	void Resize(int n) {
		Data.resize(n);
	};

	void Clear() {
		Data.clear();
	};

	void Add(T object) {
		Data.push_back(object);
	};

	void Set(int index, T object) {
		if (index < 0 || index >= Data.size())
		{
			Logger::Err("Tried to get invalix Pool index");
			return;
		}
		Data[index] = object;
	};

	T& Get(int index) {
		if (index < 0 || index >= Data.size())
		{
			Logger::Err("Tried to get invalix Pool index");
		}
		return static_cast<T&>(Data[index]);
	}

	T& operator [](unsigned int index) {
		if (index < 0 || index >= Data.size())
		{
			Logger::Err("Tried to get invalix Pool index");
		}
		return Data[index];
	}
};

// Coordinates everything
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Registry: Coordinator
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The Registry manages the creation and destruction of entities, add system and components.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Registry 
{
private:
	int numEntities = 0;
	
	// Set of entities that are flagged to be added or deleted in the next Registry update (this should be the last event of the "frame")
	std::set<Entity> EntitiesToBeAdded;
	std::set<Entity> EntitiesToBeRemoved;

	// Vector of component pools, each pool contains all data for a certain component type
	// Vector index is the component type id
	// Pool index = entity id
	std::vector<std::shared_ptr<IPool>> ComponentPools;

	// Vector of component signatures per entity, saying which component is turned on for a given entity
	// Vector index is entity id
	std::vector<Signature> EntityComponentSignatures;

	std::unordered_map<std::type_index, std::shared_ptr<System>> Systems;

public:
	Registry() { Logger::Log("Registry constructor called"); };
	~Registry() { Logger::Log("Registry destructor called"); };
	
	void Update();

	// Entity Management
	Entity CreateEntity();
	// Destroy entity
	void KillEntity(Entity entity);

	void AddEntityToSystem(Entity entity);

	// Component Management
	template <typename TComponent, typename ...TArgs> void AddComponent(Entity entity, TArgs&& ...args);

	// Remove component from Entity
	template <typename TComponent> void RemoveComponent(Entity entity);
	template <typename TComponent> bool HasComponent(Entity entity) const;
	template <typename TComponent> TComponent& GetComponent(Entity entity) const;

	// System Management
	template <typename TSystem, typename ...TArgs> void AddSystem(TArgs&& ...args);
	template <typename TSystem> void RemoveSystem();
	template <typename TSystem> bool HasSystem() const;
	template <typename TSystem> TSystem& GetSystem() const;

	// Checks the component signature of an entity and add the entity to the systems
	void AddEntityToSystems(Entity entity);
};

template <typename TComponent>
void System::RequireComponent()
{
	const auto componentId = Component<TComponent>::GetId();
	ComponentSignature.set(componentId);
}

template <typename TComponent, typename ...TArgs>
void Registry::AddComponent(Entity entity, TArgs && ...args)
{
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();

	// If the component id is greater than the current size of the ComponentPools then resize the vector
	if (componentId >= ComponentPools.size()) 
	{
		ComponentPools.resize(componentId + 1, nullptr);
	}

	// If we don't have a pool for that component type id
	if (!ComponentPools[componentId]) 
	{
		std::shared_ptr<Pool<TComponent>> newComponentPool = std::make_shared<Pool<TComponent>>();
		ComponentPools[componentId] = newComponentPool;
	}

	// Get the pool of component values for that component type
	std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(ComponentPools[componentId]);

	// If the entity id is greater than the current size of the component pool, then resize the pool
	if (entityId >= componentPool->GetSize()) 
	{
		componentPool->Resize(numEntities);
	}

	// Create a new component object of the type T and add the parameters needed
	TComponent newComponent(std::forward<TArgs>(args)...);

	// Add the new component to the component pool list
	componentPool->Set(entityId, newComponent);

	Logger::Log("Component id = " + std::to_string(componentId) + " was added to entity id = " + std::to_string(entityId));

	// Change the component signature of the entity to set the component id on the bitset to 1
	EntityComponentSignatures[entityId].set(componentId);
}

template<typename TComponent>
inline void Registry::RemoveComponent(Entity entity)
{
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();

	EntityComponentSignatures[entityId].set(componentId, false);

	Logger::Log("Component id = " + std::to_string(componentId) + " was removed from entity id = " + std::to_string(entityId));
}

template<typename TComponent>
inline bool Registry::HasComponent(Entity entity) const
{
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();

	return EntityComponentSignatures[entityId].test(componentId);
}

template<typename TComponent>
inline TComponent& Registry::GetComponent(Entity entity) const
{
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();
	auto componentPool = std::static_pointer_cast<Pool<TComponent>>(ComponentPools[componentId]);

	return componentPool->Get(entityId);
}

template<typename TSystem, typename ...TArgs>
inline void Registry::AddSystem(TArgs && ...args)
{
	std::shared_ptr<TSystem> newSystem = std::make_shared<TSystem>(std::forward<TArgs>(args)...);
	Systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));
}

template<typename TSystem>
inline void Registry::RemoveSystem()
{
	auto system = Systems.find(std::type_index(typeid(TSystem)));
	Systems.erase(system);
}

template<typename TSystem>
inline bool Registry::HasSystem() const
{
	return Systems.find(std::type_index(typeid(TSystem))) != Systems.end();
}

template<typename TSystem>
inline TSystem& Registry::GetSystem() const
{
	auto system = Systems.find(std::type_index(typeid(TSystem)));
	return *(std::static_pointer_cast<TSystem>(system->second));
}

template<typename TComponent, typename ...TArgs>
inline void Entity::AddComponent(TArgs && ...args)
{
	if (!Registry_) { return; }
	Registry_->AddComponent<TComponent>(*this, std::forward<TArgs>(args)...);
}

template<typename TComponent>
inline void Entity::RemoveComponent()
{
	if (!Registry_) { return; }
	Registry_->RemoveComponent<TComponent>(*this);
}

template<typename TComponent>
inline bool Entity::HasComponent() const
{
	if (!Registry_) { return; }
	return Registry_->HasComponent<TComponent>(*this);
}

template<typename TComponent>
inline TComponent& Entity::GetComponent() const
{
	return Registry_->GetComponent<TComponent>(*this);
}
