#ifndef MATERIAL_H
#define MATERIAL_H

#include "core/Vec3.h"
#include "core/Utils.h"
#include "lighting/Light.h"
#include "scene/SceneConfig.h"
#include "lighting/HitRecord.h"
#include "geometry/HittableList.h"
#include "lighting/ScatterRecord.h"

#include <vector>
#include <memory>
#include <cmath>
#include <cfloat>

/**
 * @class Material
 * @brief Abstract base class representing a material in a rendering system.
 *        It defines the interface for various types of materials, including methods for shading,
 *        scattering, calculating BRDF, and emitting light. Derived classes implement these methods
 *        to model specific material behaviours (e.g., diffuse, reflective, emissive).
 */
class Material
{
public:
    /**
     * @brief Virtual destructor to ensure proper cleanup of derived material classes.
     */
    virtual ~Material() = default;

    /**
     * @brief Calculates the shading of a hit point based on material properties and scene lighting.
     *        This function can be overridden by derived classes to implement specific shading models.
     * @param rec The hit record containing information about the intersection point.
     * @param view_dir The direction from the viewpoint (camera).
     * @param lights A list of lights in the scene.
     * @param scene The scene geometry (used for shadow calculations, for example).
     * @param depth The current recursion depth for reflections or refractions (unused).
     * @param config The scene configuration (unused).
     * @return The color resulting from the shading calculation.
     */
    virtual Vec3 shade(
        [[maybe_unused]] const HitRecord &rec,
        [[maybe_unused]] const Vec3 &view_dir,
        [[maybe_unused]] const std::vector<std::shared_ptr<Light>> &lights,
        [[maybe_unused]] const Hittable &scene,
        [[maybe_unused]] int depth,
        [[maybe_unused]] const SceneConfig &config) const
    {
        return Vec3(0, 0, 0); // Default: No shading
    }

    /**
     * @brief Scatters an incoming ray and calculates the new direction for the scattered ray.
     *        This function can be overridden by derived classes to implement specific scattering behaviors
     *        (e.g., reflection, refraction).
     * @param ray_in The incoming ray.
     * @param rec The hit record containing information about the intersection point.
     * @param scatter_direction The direction of the scattered ray (output).
     * @param pdf The probability density function value (output, unused).
     * @return The color of the scattered ray (typically used for reflection or refraction).
     */
    virtual Vec3 scatter(
        [[maybe_unused]] const Ray &ray_in,
        [[maybe_unused]] const HitRecord &rec,
        [[maybe_unused]] Vec3 &scatter_direction,
        [[maybe_unused]] float &pdf) const
    {
        return false; // Default: No scattering
    }

    /**
     * @brief Another version of the scatter function, which stores scattering information in a `ScatterRecord`.
     *        This function is typically used to handle materials that involve both reflection and refraction.
     * @param ray_in The incoming ray.
     * @param rec The hit record containing information about the intersection point.
     * @param scatter_rec A reference to a `ScatterRecord` that stores the result of the scattering.
     * @return True if the scattering was successful, false otherwise.
     */
    virtual bool scatter([[maybe_unused]] const Ray &ray_in,
                         [[maybe_unused]] const HitRecord &rec,
                         [[maybe_unused]] ScatterRecord &scatter_rec) const
    {
        return false; // Default: No scattering
    }

    /**
     * @brief Calculates the Bidirectional Reflectance Distribution Function (BRDF) for the material.
     *        This function can be overridden by derived classes to implement specific BRDF models, such as
     *        Lambertian (diffuse), Phong, or Cook-Torrance for more complex materials.
     * @param rec The hit record containing information about the intersection.
     * @param view_dir The direction from the viewpoint (camera).
     * @param light_dir The direction from the light source.
     * @return The BRDF value representing how the material interacts with light.
     */
    virtual Vec3 brdf([[maybe_unused]] const HitRecord &rec,
                      [[maybe_unused]] const Vec3 &view_dir,
                      [[maybe_unused]] const Vec3 &light_dir) const
    {
        return Vec3(0, 0, 0); // Default: No BRDF contribution
    }

    /**
     * @brief Calculates the emitted light from the material. This is used for light sources like emissive materials.
     *        For non-emissive materials, this function should return a zero vector.
     * @param rec The hit record containing information about the intersection.
     * @param u The u-coordinate of the surface (typically used for texture mapping, unused here).
     * @param v The v-coordinate of the surface (typically used for texture mapping, unused here).
     * @param p The point of intersection on the surface.
     * @return The emitted light color.
     */
    virtual Vec3 emitted([[maybe_unused]] const HitRecord &rec,
                         [[maybe_unused]] float u,
                         [[maybe_unused]] float v,
                         [[maybe_unused]] const Vec3 &p) const
    {
        return Vec3(0, 0, 0); // Default: No emission
    }
};

#endif // MATERIAL_H
