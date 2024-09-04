#include <memory>
#include <iostream>

#include "Graphics/Window.h"
#include "Graphics/Renderer.h"
#include "Util/Filesystem.h"
#include "Input/InputManager.h"
#include "Assets/AssetManager.h"
#include "ECS/ECSManager.h"

#include "CommonAssets.h"
#include "CommonComponents.h"
#include "CommonSystems.h"

using namespace engine;

int main() {
    auto window = Window("Test Engine", 1920, 1080, false, false);
    auto renderer = Renderer(&window);

    auto inputManager = InputManager(&window);
    auto assetManager = AssetManager();
    auto ecsManager = ECSManager();

    // Setup
    renderer.initialize();
    ecsManager.registerSystem(std::make_unique<systems::TransformSystem>());
    ecsManager.registerSystem(std::make_unique<systems::RenderSystem>());
    ecsManager.registerSystem(std::make_unique<systems::FreeFlySystem>());

    std::shared_ptr<assets::Program> shader = assetManager.loadAsset<assets::Program>(getAssetsDir().append("default.vert"));
    std::shared_ptr<assets::Mesh> mesh = assetManager.loadAsset<assets::Mesh>(getAssetsDir().append("cube/Cube.gltf"));

    // Main loop
    while (window.getIsOpen()) {
        renderer.clear();

        inputManager.processEvents();
        ecsManager.updateSystems(window.getDeltaTime());

        renderer.present();
    }
}