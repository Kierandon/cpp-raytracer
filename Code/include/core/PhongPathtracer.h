#ifndef PhongPathtracer_H
#define PhongPathtracer_H

#include "core/Ray.h"
#include "geometry/Hittable.h"
#include "lighting/HitRecord.h"
#include "lighting/Light.h"
#include "scene/SceneConfig.h"
#include "core/Vec3.h"

#include <vector>

/**
 * @class PhongPathtracer
 * @brief A class that implements a path tracer using the Phong reflection model.
 *
 * This class is responsible for tracing rays through a scene and computing the color
 * of each pixel using the Phong reflection model. It supports recursive ray tracing
 * up to a specified maximum depth.
 */
class PhongPathtracer
{
public:
    /**
     * @brief Constructs a PhongPathtracer with a specified maximum recursion depth.
     *
     * @param max_depth The maximum recursion depth for ray tracing.
     */
    PhongPathtracer(int max_depth);

    /**
     * @brief Traces a ray through the scene and computes the color at the intersection point.
     *
     * @param ray The ray to trace.
     * @param scene The scene to trace the ray through.
     * @param depth The current recursion depth.
     * @param config The scene configuration.
     * @param lights The list of lights in the scene.
     * @return The computed color at the intersection point.
     */
    Vec3 trace(const Ray &ray, const Hittable &scene, int depth, SceneConfig &config, const std::vector<std::shared_ptr<Light>> lights) const;

private:
    int max_depth; // Maximum recursion depth
};

#endif // PhongPathtracer_H
