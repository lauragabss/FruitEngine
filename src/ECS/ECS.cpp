#include "ECS.h"

int IComponent::nextId = 0;

void System::AddEntityToSystem(Entity entity)
{
	Entities.push_back(entity);
}

void System::RemoveEntityFromSystem(Entity entity)
{
	// iterate through the array and erase the correct entity, also using lambda function
	Entities.erase(std::remove_if(Entities.begin(), Entities.end(), [&entity](Entity other) 
		{
			return entity == other; 
		}),
	Entities.end());
}

std::vector<Entity> System::GetSystemEntities() const
{
	return Entities;
}

const Signature& System::GetComponentSignature() const
{
	return ComponentSignature;
}

void Registry::Update()
{
	// Add entities that are waiting to be created in the system
	for (auto entity: EntitiesToBeAdded) 
	{
		AddEntityToSystems(entity);
	}
	EntitiesToBeAdded.clear();

	// TODO: Remove entities that are waiting to be removed from the system
}

Entity Registry::CreateEntity()
{
	int entityId = numEntities++;
	if (entityId >= EntityComponentSignatures.size())
	{
		EntityComponentSignatures.resize(entityId + 1);
	}
	Entity entity(entityId);
	entity.Registry_ = this;
	EntitiesToBeAdded.insert(entity);

	Logger::Log("Entity created with id = " + std::to_string(entityId));

	return entity;
}

void Registry::KillEntity(Entity entity)
{
	EntitiesToBeRemoved.insert(entity);
}

void Registry::AddEntityToSystem(Entity entity)
{
}

void Registry::AddEntityToSystems(Entity entity)
{
	const auto entityId = entity.GetId();

	const auto entityComponentSignature = EntityComponentSignatures[entityId];
	for (auto& system: Systems) 
	{
		const auto& systemComponentSignature = system.second->GetComponentSignature();

		// Checks if the entity component signature has all the components required by this system
		bool isInterested = (entityComponentSignature & systemComponentSignature) == systemComponentSignature;

		if (isInterested)
		{
			system.second->AddEntityToSystem(entity);
		}
	}
}
