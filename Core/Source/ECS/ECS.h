#pragma once

#include <spdlog/spdlog.h>

#include <cstdint>
#include <bitset>
#include <cstdlib>

#define SPDLOG_ASSERT(condition, ...) \
    if (!(condition)) { \
        spdlog::critical("Assertion Failed: " __VA_ARGS__); \
        std::abort(); \
    }

using Entity = std::uint32_t;
const Entity MAX_ENTITIES = 1000000;

using ComponentType = std::uint8_t;
const ComponentType MAX_COMPONENTS = 32;
using Signature = std::bitset<MAX_COMPONENTS>;

class IComponentArray 
{
public:
    virtual ~IComponentArray() = default;
    virtual void EntityDestroyed(Entity entity) = 0;
};

template<typename T>
class ComponentArray : public IComponentArray
{
private:
    T denseData[MAX_ENTITIES]{};            // The actual component payload
    Entity denseEntities[MAX_ENTITIES]{};   // The Entity ID that owns the payload
    size_t sparseIndices[MAX_ENTITIES]{};   // Points to the correct Dense Array index
    size_t activeCount;                     // How many components are currently alive

public:
    ComponentArray() 
    {
        activeCount = 0;
    }

    void EntityDestroyed(Entity entity) override 
    {
        if (sparseIndices[entity] < activeCount && denseEntities[sparseIndices[entity]] == entity) {
            RemoveData(entity);
        }
    }

    void InsertData(Entity entity, T component) 
    {
        SPDLOG_ASSERT(activeCount < MAX_ENTITIES, "Too many entities in ComponentArray.");
        denseData[activeCount] = component;
        denseEntities[activeCount] = entity;
        sparseIndices[entity] = activeCount;
        ++activeCount;
    }

    void RemoveData(Entity entity) 
    {
        SPDLOG_ASSERT(activeCount > 0, "Cannot remove from an empty ComponentArray.");
        size_t indexOfRemoved = sparseIndices[entity];
        size_t indexOfLast = activeCount - 1;
        denseData[indexOfRemoved] = denseData[indexOfLast];
        Entity ownerOfLast = denseEntities[indexOfLast];
        denseEntities[indexOfRemoved] = ownerOfLast;
        sparseIndices[ownerOfLast] = indexOfRemoved;
        --activeCount;
    }

    T& GetData(Entity entity) 
    {
        SPDLOG_ASSERT(sparseIndices[entity] < activeCount, "Retrieving non-existent component.");
        return denseData[sparseIndices[entity]];
    }
};