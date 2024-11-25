#ifndef DIFFUSE_H
#define DIFFUSE_H

#include "materials/Material.h"
#include "materials/CosinePDF.h"

/**
 * @class Diffuse
 * @brief Represents a diffuse material, typically used to simulate Lambertian reflection,
 *        where light is scattered equally in all directions.
 *        This material interacts with light based on the albedo and uses a cosine-weighted probability distribution
 *        for the scattered rays.
 */
class Diffuse : public Material
{
public:
    /**
     * @brief Constructs a Diffuse material with a specified albedo (diffuse color).
     * @param albedo The albedo (diffuse color) of the material.
     */
    Diffuse(const Vec3 &albedo) : m_albedo(albedo) {}

    /**
     * @brief Scatters an incoming ray by generating a new random direction based on the cosine-weighted PDF.
     *        This method models Lambertian reflection, where light is scattered uniformly across the surface.
     * @param ray_in The incoming ray.
     * @param rec The hit record containing information about the intersection point.
     * @param scatter_rec A reference to a ScatterRecord that will store the result of the scattering event.
     * @return True if the scattering was successful, false otherwise.
     */
    bool scatter(const Ray &ray_in,
                 const HitRecord &rec,
                 ScatterRecord &scatter_rec) const override;

    /**
     * @brief Calculates the Bidirectional Reflectance Distribution Function (BRDF) for the diffuse material.
     *        In this case, the BRDF is a constant, as the diffuse reflection is isotropic and does not depend on
     *        the incident or outgoing directions.
     * @param rec The hit record containing information about the intersection.
     * @param view_dir The direction from the viewpoint (camera).
     * @param light_dir The direction from the light source.
     * @return The BRDF value representing how the material interacts with light.
     */
    Vec3 brdf(const HitRecord &rec,
              const Vec3 &view_dir,
              const Vec3 &light_dir) const override;

private:
    Vec3 m_albedo; ///< The albedo (diffuse color) of the material, which determines how light is absorbed and scattered.
};

#endif // DIFFUSE_H
