#include <vroom/core/Engine.hpp>

int main() {
    // Use default config (headless=false)
    vroom::Engine engine;

    // Create editor scene
    auto editorScene = std::make_shared<vroom::Scene>();

    engine.getSceneManager().loadScene(editorScene);

    // Run engine
    engine.run();

    return 0;
}
