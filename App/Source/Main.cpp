#include <Core/Application.h>
#include <ECS/Coordinator.h>
#include "Layers/StressTestLayer.h"
#include "Constants.h"

Coordinator coordinator;

int main() {
    Core::ApplicationSpecification appSpec;
    appSpec.name = "tecs";
    appSpec.windowSpec.title = "tecs Stress Test";

    Core::Application app(appSpec);
    app.PushLayer<StressTestLayer>();
    app.Run();

    return 0;
}