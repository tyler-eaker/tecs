#pragma once

#include "ECS.h"

#include <queue>

class EntityManager 
{
private:
	std::queue<Entity> availableEntities;
	Signature signatures[MAX_ENTITIES];
	uint32_t livingEntityCount;

public:
	EntityManager() 
	{
		for (Entity entity = 0; entity < MAX_ENTITIES; ++entity) {
			availableEntities.push(entity);
		}
		livingEntityCount = 0;
	}

	Entity CreateEntity() 
	{
		SPDLOG_ASSERT(livingEntityCount < MAX_ENTITIES, "Max number of entities exceeded.");
		Entity newID = availableEntities.front();
		availableEntities.pop();
		++livingEntityCount;
		return newID;
	}

	void DestroyEntity(Entity entity) 
	{
		SPDLOG_ASSERT(entity < MAX_ENTITIES, "Entity out of range.");
		signatures[entity].reset();
		availableEntities.push(entity);
		--livingEntityCount;
	}

	void SetSignature(Entity entity, Signature signature) 
	{
		SPDLOG_ASSERT(entity < MAX_ENTITIES, "Entity out of range.");
		signatures[entity] = signature;
	}

	Signature GetSignature(Entity entity) 
	{
		SPDLOG_ASSERT(entity < MAX_ENTITIES, "Entity out of range.");
		return signatures[entity];
	}
};