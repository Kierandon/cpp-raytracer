#include "scene/Scene.h"
#include "scene/SceneRenderer.h"
#include "scene/SceneLoader.h"

#include <memory>
#include <iostream>

int main(int argc, char *argv[])
{
    Scene scene;
    SceneConfig config;
    SceneLoader loader;
    SceneRenderer renderer;

    if (argc > 1)
    {
        loader.load_scene_from_json(scene, config, argv[1]);
    }
    else
    {
        loader.load_default_scene(scene, config);
    }

    std::string outputFile = "output.ppm";
    renderer.render(scene, config, outputFile);

    return 0;
}
