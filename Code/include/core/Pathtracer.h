#ifndef PATHTRACER_H
#define PATHTRACER_H

#include "core/Ray.h"
#include "geometry/Hittable.h"
#include "lighting/HitRecord.h"
#include "lighting/ScatterRecord.h"
#include "materials/Material.h"
#include "lighting/Light.h"
#include "scene/SceneConfig.h"
#include "core/Vec3.h"

#include <random>
#include <vector>
#include <algorithm>

/**
 * @class Pathtracer
 * @brief A class for performing path tracing in a 3D scene.
 *
 * The Pathtracer class is responsible for tracing rays through a scene and computing the resulting color
 * by simulating the interaction of light with objects in the scene.
 */
class Pathtracer
{
public:
    /**
     * @brief Constructs a Pathtracer object with the given scene configuration.
     * @param config Reference to the scene configuration.
     */
    Pathtracer(SceneConfig &config) : scene_config(config) {}

    /**
     * @brief Traces a ray through the scene and computes the resulting color.
     * @param ray The ray to trace.
     * @param world The world containing the objects to be hit by the ray.
     * @param depth The current recursion depth for the ray tracing.
     * @param lights The lights in the scene.
     * @return The computed color as a Vec3.
     */
    Vec3 trace(const Ray &ray, const Hittable &world, int depth, const std::vector<std::shared_ptr<Light>> &lights);

private:
    SceneConfig &scene_config;

    /**
     * @brief Computes the background color for a given ray.
     * @param ray The ray for which to compute the background color.
     * @return The background color as a Vec3.
     */
    Vec3 background_color(const Ray &ray);

    /**
     * @brief Computes the direct lighting at a hit point.
     * @param rec The hit record containing information about the hit point.
     * @param world The world containing the objects to be hit by the ray.
     * @param incident_ray The incident ray that hit the object.
     * @param lights The lights in the scene.
     * @return The computed direct lighting as a Vec3.
     */
    Vec3 compute_direct_lighting(const HitRecord &rec,
                                 const Hittable &world,
                                 const Ray &incident_ray,
                                 const std::vector<std::shared_ptr<Light>> &lights);

    /**
     * @brief Clamps the radiance value to a maximum value.
     * @param v The radiance value to clamp.
     * @param max_value The maximum value to clamp to (default is 100.0f).
     * @return The clamped radiance value as a Vec3.
     */
    Vec3 clamp_radiance(const Vec3 &v, float max_value = 100.0f);
};

#endif // PATHTRACER_H
