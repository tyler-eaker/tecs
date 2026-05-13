#pragma once

#include "Window.h"
#include "ECS/Coordinator.h"
#include "Systems/PhysicsSystem.h"
#include "Systems/RenderSystem.h"

#include <string>
#include <memory>
#include <vector>

namespace Core {

	struct ApplicationSpecification {
		const char* name = "Application";
		WindowSpecification windowSpec;
	};

	class Application
	{
	public:
		Application(const ApplicationSpecification& specification = ApplicationSpecification());
		~Application();

		void Run();
		void Stop();

		std::shared_ptr<Window> GetWindow() const { return m_Window; }

		static Application& Get();
		static float GetTime();

	private:
		ApplicationSpecification m_Specification;
		std::shared_ptr<Window> m_Window;
		bool m_Running = false;

		std::shared_ptr<PhysicsSystem> m_PhysicsSystem;
		std::shared_ptr<RenderSystem> m_RenderSystem;
		std::vector<Entity> m_ActiveSwarm;
		int m_TargetHundreds = 1;
		bool m_PhysicsEnabled = true;
		int m_FrameCount = 0;
	};
}