#include "Application.h"
#include "Constants.h"
#include "Components.h"

#include <raylib.h>
#include <rlImGui.h>
#include <imgui.h>
#include <algorithm> // Required for std::clamp

// Connect to the global ECS coordinator
extern Coordinator coordinator;

namespace Core {

	static Application* s_Application = nullptr;

	Application::Application(const ApplicationSpecification& specification)
		: m_Specification(specification)
	{
		SetTraceLogLevel(LOG_WARNING);

		SPDLOG_ASSERT(!s_Application, "Application already exists!");
		s_Application = this;

		if (m_Specification.windowSpec.title == nullptr || m_Specification.windowSpec.title[0] == '\0') {
			m_Specification.windowSpec.title = m_Specification.name;
		}

		m_Window = std::make_shared<Window>(m_Specification.windowSpec);
		m_Window->Create();

		rlImGuiSetup(true);

		coordinator.RegisterComponent<Position>();
		coordinator.RegisterComponent<Velocity>();
		coordinator.RegisterComponent<Sprite>();

		m_PhysicsSystem = coordinator.RegisterSystem<PhysicsSystem>();
		Signature physicsSignature;
		physicsSignature.set(coordinator.GetComponentType<Position>());
		physicsSignature.set(coordinator.GetComponentType<Velocity>());
		coordinator.SetSystemSignature<PhysicsSystem>(physicsSignature);

		m_RenderSystem = coordinator.RegisterSystem<RenderSystem>();
		Signature renderSignature;
		renderSignature.set(coordinator.GetComponentType<Position>());
		renderSignature.set(coordinator.GetComponentType<Sprite>());
		coordinator.SetSystemSignature<RenderSystem>(renderSignature);
	}

	Application::~Application() {
		rlImGuiShutdown();
		m_Window->Destroy();
		s_Application = nullptr;
	}

	void Application::Run() {
		m_Running = true;
		float lastTime = GetTime();

		// Main Application loop
		while (m_Running) {

			if (m_Window->ShouldClose()) {
				Stop();
				break;
			}

			float currentTime = GetTime();
			float timestep = std::clamp(currentTime - lastTime, 0.001f, 0.1f);
			lastTime = currentTime;

			int targetEntities = m_TargetHundreds * 100;

			while (m_ActiveSwarm.size() < static_cast<uint32_t>(targetEntities)) {
				Entity entity = coordinator.CreateEntity();
				coordinator.AddComponent<Position>(entity, Position{ static_cast<float>(GetRandomValue(0, SCREEN_WIDTH)), static_cast<float>(GetRandomValue(0, SCREEN_HEIGHT)) });
				coordinator.AddComponent<Velocity>(entity, Velocity{ static_cast<float>(GetRandomValue(-500.0f, 500.0f)), static_cast<float>(GetRandomValue(-50.0f, 50.0f)) });
				coordinator.AddComponent<Sprite>(entity, Sprite{ static_cast<uint32_t>(GetRandomValue(1, 5)), static_cast<uint32_t>(GetRandomValue(1, 5)), COLORS[GetRandomValue(0, MAX_COLORS_COUNT - 1)] });
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

			m_Window->BeginFrame();
			ClearBackground(CLEAR_BACKGROUND_COLOR);

			m_RenderSystem->Draw();

			rlImGuiBegin();
			ImGui::Begin("tecs");
			ImGui::Text("Current FPS: %d", GetFPS());
			ImGui::Text("Frame %d", m_FrameCount);
			ImGui::Text("Entity count: %zu", m_ActiveSwarm.size());
			ImGui::SliderInt("Swarm Size", &m_TargetHundreds, 1, MAX_ENTITIES / 100);
			ImGui::Checkbox("Enable physics", &m_PhysicsEnabled);
			ImGui::End();
			rlImGuiEnd();

			m_Window->EndFrame();

			m_FrameCount++;
		}
	}

	void Application::Stop() {
		m_Running = false;
	}

	Application& Application::Get() {
		SPDLOG_ASSERT(s_Application != nullptr, "Application instance is null!");
		return *s_Application;
	}

	float Application::GetTime() {
		return static_cast<float>(::GetTime());
	}
}