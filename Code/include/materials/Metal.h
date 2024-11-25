#ifndef METAL_H
#define METAL_H

#include "core/Vec3.h"
#include "core/Utils.h"
#include "materials/Material.h"

/**
 * @class Metal
 * @brief Represents a metallic material with a specified albedo (diffuse color) and roughness.
 *        The material behaves according to the physics of metallic reflection, where light reflects off
 *        the surface and is affected by the surface roughness. This material type can model shiny metallic
 *        surfaces like polished metals or rougher finishes like brushed metal.
 */
class Metal : public Material
{
public:
    /**
     * @brief Constructs a Metal material with a specified albedo (color) and roughness.
     * @param albedo The color of the material, which determines the color of reflected light.
     * @param roughness The roughness factor, which controls how glossy or diffuse the reflections are.
     */
    Metal(const Vec3 &albedo, float roughness)
        : m_albedo(albedo), m_roughness(roughness) {}

    /**
     * @brief Scatters an incoming ray based on the material's reflection properties.
     *        This models the reflection of light on a metallic surface, considering the surface roughness.
     * @param ray_in The incoming ray.
     * @param rec The hit record containing information about the intersection point.
     * @param scatter_rec A reference to a ScatterRecord that stores the result of the scattering.
     * @return True if the scattering was successful, false otherwise.
     */
    bool scatter(const Ray &ray_in,
                 const HitRecord &rec,
                 ScatterRecord &scatter_rec) const override;

    /**
     * @brief Calculates the Bidirectional Reflectance Distribution Function (BRDF) for the metal material.
     *        The BRDF models how light is reflected on a metallic surface, taking into account the view and light directions,
     *        as well as the material's roughness and albedo.
     * @param rec The hit record containing information about the intersection.
     * @param view_dir The direction from the viewpoint (camera).
     * @param light_dir The direction from the light source.
     * @return The BRDF value representing how the material interacts with light.
     */
    Vec3 brdf(const HitRecord &rec,
              const Vec3 &view_dir,
              const Vec3 &light_dir) const override;

private:
    Vec3 m_albedo;     ///< The color of the metal, which affects the color of the reflected light.
    float m_roughness; ///< The roughness of the metal, which determines how glossy or diffuse the reflections are.
};

#endif // METAL_H
