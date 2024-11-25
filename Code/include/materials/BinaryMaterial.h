#ifndef BINARY_MATERIAL_H
#define BINARY_MATERIAL_H

#include "materials/Material.h"

/**
 * @class BinaryMaterial
 * @brief Represents a simple material that returns a constant color when shaded or scattered.
 *        This material is useful for testing or debugging purposes where a uniform color is desired.
 */
class BinaryMaterial : public Material
{
public:
    /**
     * @brief Constructs a BinaryMaterial with a specified hit color.
     * @param hit_color The color to be returned when the material interacts with a ray (default: red).
     */
    BinaryMaterial(const Vec3 &hit_color = Vec3(1.0f, 0.0f, 0.0f))
        : hit_color(hit_color) {}

    /**
     * @brief Shades a hit point, returning the constant hit color.
     *        This method is used in the shading process to calculate the final color at an intersection.
     * @param rec The hit record containing information about the intersection.
     * @param view_dir The direction from the point of view.
     * @param lights A list of lights in the scene.
     * @param scene The scene geometry for additional lighting interactions.
     * @param depth The current recursion depth for reflections or refractions (unused).
     * @param config The scene configuration for additional settings (unused).
     * @return The constant hit color for the material.
     */
    virtual Vec3 shade(
        [[maybe_unused]] const HitRecord &rec,
        [[maybe_unused]] const Vec3 &view_dir,
        [[maybe_unused]] const std::vector<std::shared_ptr<Light>> &lights,
        [[maybe_unused]] const Hittable &scene,
        [[maybe_unused]] int depth,
        [[maybe_unused]] const SceneConfig &config) const
    {
        return hit_color; ///< Return the constant color of the material.
    }

    /**
     * @brief Scatters a ray, returning the constant hit color.
     *        This method calculates the scattered direction and PDF for the ray interaction.
     * @param ray_in The incoming ray interacting with the material.
     * @param rec The hit record containing information about the intersection.
     * @param scatter_direction The direction the scattered ray will take (unused).
     * @param pdf The probability density function value (unused).
     * @return The constant hit color for the material.
     */
    virtual Vec3 scatter(
        [[maybe_unused]] const Ray &ray_in,
        [[maybe_unused]] const HitRecord &rec,
        [[maybe_unused]] Vec3 &scatter_direction,
        [[maybe_unused]] float &pdf) const override
    {
        return hit_color; ///< Return the constant color of the material.
    }

private:
    Vec3 hit_color; ///< The constant color of the material, used for shading and scattering.
};

#endif // BINARY_MATERIAL_H
