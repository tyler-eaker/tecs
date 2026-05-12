#pragma once

#include "ECS.h"
#include "EntityManager.h"
#include "SystemManager.h"
#include <unordered_map>
#include <memory>
#include <typeinfo>

class Coordinator {
private:
	std::unique_ptr<EntityManager> entityManager;
	std::unique_ptr<SystemManager> systemManager;

	std::unordered_map<const char*, std::shared_ptr<IComponentArray>> componentArrays;

	std::unordered_map<const char*, ComponentType> componentTypes;
	ComponentType nextComponentType = 0;

	template<typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray() {
		const char* typeName = typeid(T).name();
		SPDLOG_ASSERT(componentArrays.find(typeName) != componentArrays.end(), "Component not registered before use.");
		return std::static_pointer_cast<ComponentArray<T>>(componentArrays[typeName]);
	}

public:
	Coordinator() {
		entityManager = std::make_unique<EntityManager>();
		systemManager = std::make_unique<SystemManager>();
	}

	template<typename T>
	void RegisterComponent() {
		const char* typeName = typeid(T).name();
		SPDLOG_ASSERT(componentArrays.find(typeName) == componentArrays.end(), "Registering component type more than once.");
		componentTypes[typeName] = nextComponentType;
		componentArrays[typeName] = std::make_shared<ComponentArray<T>>();
		++nextComponentType;
	}

	template<typename T>
	ComponentType GetComponentType() {
		const char* typeName = typeid(T).name();
		SPDLOG_ASSERT(componentTypes.find(typeName) != componentTypes.end(), "Component not registered before use.");
		return componentTypes[typeName];
	}

	Entity CreateEntity() {
		return entityManager->CreateEntity();
	}

	void DestroyEntity(Entity entity) {
		entityManager->DestroyEntity(entity);
		for (auto const& pair : componentArrays) {
			auto const& componentArray = pair.second;
			componentArray->EntityDestroyed(entity);
		}
		systemManager->EntityDestroyed(entity);
	}

	template<typename T>
	void AddComponent(Entity entity, T component) {
		GetComponentArray<T>()->InsertData(entity, component);
		Signature signature = entityManager->GetSignature(entity);
		signature.set(GetComponentType<T>(), true);
		entityManager->SetSignature(entity, signature);
		systemManager->EntitySignatureChanged(entity, signature);
	}

	template<typename T>
	void RemoveComponent(Entity entity) {
		GetComponentArray<T>()->RemoveData(entity);
		Signature signature = entityManager->GetSignature(entity);
		signature.set(GetComponentType<T>(), false);
		entityManager->SetSignature(entity, signature);
		systemManager->EntitySignatureChanged(entity, signature);
	}

	template<typename T>
	T& GetComponent(Entity entity) {
		return GetComponentArray<T>()->GetData(entity);
	}

	template<typename T>
	std::shared_ptr<T> RegisterSystem() {
		return systemManager->RegisterSystem<T>();
	}

	template<typename T>
	void SetSystemSignature(Signature signature) {
		systemManager->SetSignature<T>(signature);
	}
};