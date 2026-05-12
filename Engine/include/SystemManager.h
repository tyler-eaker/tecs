#pragma once
#include "ECS.h"
#include "System.h"
#include <unordered_map>
#include <memory>

class SystemManager {
private:
    std::unordered_map<const char*, Signature> signatures;
    std::unordered_map<const char*, std::shared_ptr<System>> systems;

public:
    template<typename T>
    std::shared_ptr<T> RegisterSystem() {
        const char* typeName = typeid(T).name();
        SPDLOG_ASSERT(systems.find(typeName) == systems.end(), "Registering system more than once.");

        auto system = std::make_shared<T>();
        systems[typeName] = system;
        return system;
    }

    template<typename T>
    void SetSignature(Signature signature) {
        const char* typeName = typeid(T).name();
        SPDLOG_ASSERT(systems.find(typeName) != systems.end(), "System used before registered.");
        signatures[typeName] = signature;
    }

    void EntityDestroyed(Entity entity) {
        for (auto const& pair : systems) {
            auto const& system = pair.second;
            system->entities.erase(entity);
        }

    }

    void EntitySignatureChanged(Entity entity, Signature entitySignature) {
        for (auto const& pair : systems) {
            const char* typeName = pair.first;
            auto const& system = pair.second;
            auto systemSignature = signatures[typeName];
            if ((entitySignature & systemSignature) == systemSignature) {
                system->entities.insert(entity);
            }
            else {
                system->entities.erase(entity);
            }
        }
    }
};