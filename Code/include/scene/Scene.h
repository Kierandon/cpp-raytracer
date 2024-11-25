#ifndef SCENE_H
#define SCENE_H

#include "scene/SceneConfig.h"
#include "core/Camera.h"
#include "core/Image.h"
#include "geometry/Hittable.h"
#include "lighting/Light.h"

#include <memory>
#include <vector>

/**
 * @struct Scene
 * @brief Represents a scene in a 3D rendering system.
 *        The scene contains the geometric objects, light sources, camera, and image for rendering. 
 *        It serves as the container for all the components needed to generate a rendered image.
 */
struct Scene
{
    /**
     * @brief A list of objects in the scene that can be hit by rays, such as meshes, spheres, and other geometric entities.
     */
    std::vector<std::shared_ptr<Hittable>> objects;

    /**
     * @brief A list of light sources in the scene that illuminate the objects. Lights are used to compute shading.
     */
    std::vector<std::shared_ptr<Light>> lights;

    /**
     * @brief The root object of the scene, used for organizing the scene hierarchy. This is often the "root" of the scene graph.
     */
    std::shared_ptr<Hittable> scene_root;

    /**
     * @brief The camera that defines the viewpoint for rendering. The camera controls the perspective, field of view, and other visual properties.
     */
    std::shared_ptr<Camera> camera;

    /**
     * @brief The image that will store the rendered output of the scene, representing the final result of the rendering process.
     */
    std::shared_ptr<Image> image;
};

#endif // SCENE_H
