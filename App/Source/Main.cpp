#include <Core/Application.h>
#include <ECS/Coordinator.h>
#include "Constants.h"

// The single global ECS coordinator
Coordinator coordinator;

int main() {
    Core::ApplicationSpecification spec;
    spec.name = "tecs";
    spec.windowSpec.title = "tecs Engine";
    spec.windowSpec.width = SCREEN_WIDTH;
    spec.windowSpec.height = SCREEN_HEIGHT;
    spec.windowSpec.fpsLimit = FPS_LIMIT;

    Core::Application app(spec);
    app.Run();

    return 0;
}