#pragma once
#include <Core/Layer.h>
#include <Core/AssetManager.h>
#include <ECS/Coordinator.h>
#include <raylib.h>
#include <imgui.h>
#include "Systems/PhysicsSystem.h"
#include "Systems/RenderSystem.h"
#include "Components.h"
#include "Constants.h"
#include <vector>

extern Coordinator coordinator;

class StressTestLayer : public Core::Layer
{
public:
	StressTestLayer() : Layer("StressTestLayer") {}

	void OnAttach() override
	{
		coordinator.RegisterComponent<Position>();
		coordinator.RegisterComponent<Velocity>();
		coordinator.RegisterComponent<Sprite>();

		m_PhysicsSystem = coordinator.RegisterSystem<PhysicsSystem>();
		Signature physicsSig;
		physicsSig.set(coordinator.GetComponentType<Position>());
		physicsSig.set(coordinator.GetComponentType<Velocity>());
		coordinator.SetSystemSignature<PhysicsSystem>(physicsSig);

		m_RenderSystem = coordinator.RegisterSystem<RenderSystem>();
		Signature renderSig;
		renderSig.set(coordinator.GetComponentType<Position>());
		renderSig.set(coordinator.GetComponentType<Sprite>());
		coordinator.SetSystemSignature<RenderSystem>(renderSig);

		Core::AssetManager::Get().LoadTexture("test", "App/Assets/test.png");
	}

	void OnUpdate(float timestep) override
	{
		if (m_TargetHundreds > (MAX_ENTITIES / 100)) m_TargetHundreds = MAX_ENTITIES / 100;
		if (m_TargetHundreds < 1) m_TargetHundreds = 1;

		int targetEntities = m_TargetHundreds * 100;

		while (m_ActiveSwarm.size() < static_cast<uint32_t>(targetEntities)) {
			Entity entity = coordinator.CreateEntity();
			coordinator.AddComponent<Position>(entity, Position{ static_cast<float>(GetRandomValue(0, SCREEN_WIDTH)), static_cast<float>(GetRandomValue(0, SCREEN_HEIGHT)) });
			coordinator.AddComponent<Velocity>(entity, Velocity{ static_cast<float>(GetRandomValue(-500.0f, 500.0f)), static_cast<float>(GetRandomValue(-50.0f, 50.0f)) });

			/*coordinator.AddComponent<Sprite>(entity, Sprite{
				"test",
				static_cast<uint32_t>(GetRandomValue(10, 30)),
				static_cast<uint32_t>(GetRandomValue(10, 30)),
				COLORS[GetRandomValue(0, MAX_COLORS_COUNT - 1)]
				});*/
			coordinator.AddComponent<Sprite>(entity, Sprite{
				"test",
				32,
				32,
				WHITE
				});
			m_ActiveSwarm.push_back(entity);
		}

		while (m_ActiveSwarm.size() > static_cast<uint32_t>(targetEntities)) {
			Entity entityToDestroy = m_ActiveSwarm.back();
			coordinator.DestroyEntity(entityToDestroy);
			m_ActiveSwarm.pop_back();
		}

		if (m_PhysicsEnabled) {
			m_PhysicsSystem->Update(timestep, SCREEN_WIDTH, SCREEN_HEIGHT);
		}
	}

	void OnRender() override
	{
		m_RenderSystem->Draw();
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("ECS Stress Test");
		ImGui::Text("Current FPS: %d", GetFPS());
		ImGui::Text("Entity count: %zu", m_ActiveSwarm.size());
		ImGui::PushButtonRepeat(true);
		if (ImGui::Button("-") && m_TargetHundreds > 1) {
			m_TargetHundreds--;
		}
		ImGui::SameLine();
		ImGui::SetNextItemWidth(150.0f);
		ImGui::SliderInt("##SwarmSlider", &m_TargetHundreds, 1, MAX_ENTITIES / 100);
		ImGui::SameLine();
		if (ImGui::Button("+") && m_TargetHundreds < (MAX_ENTITIES / 100)) {
			m_TargetHundreds++;
		}
		ImGui::PopButtonRepeat();
		ImGui::SameLine();
		ImGui::Text("Swarm Size");
		ImGui::Checkbox("Enable physics", &m_PhysicsEnabled);
		ImGui::End();
	}
private:
	std::shared_ptr<PhysicsSystem> m_PhysicsSystem;
	std::shared_ptr<RenderSystem> m_RenderSystem;
	std::vector<Entity> m_ActiveSwarm;

	int m_TargetHundreds = 0;
	bool m_PhysicsEnabled = true;
};