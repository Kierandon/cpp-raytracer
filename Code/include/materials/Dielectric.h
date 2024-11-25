#ifndef DIELECTRIC_H_
#define DIELECTRIC_H_

#include "core/Vec3.h"
#include "materials/Material.h"
#include "core/Utils.h"
#include "lighting/ScatterRecord.h"

/**
 * @class Dielectric
 * @brief Represents a dielectric material, typically used to simulate transparent or refractive materials like glass or water.
 *        It handles refraction based on the material's index of refraction (IOR), and supports scattering for rays interacting with the material.
 */
class Dielectric : public Material
{
public:
    /**
     * @brief Constructs a Dielectric material with a specified index of refraction (IOR).
     * @param ior The index of refraction for the material (e.g., 1.5 for glass).
     */
    Dielectric(float ior) : m_ior(ior) {}

    /**
     * @brief Scatters an incoming ray by calculating the refracted direction based on Snell's law.
     *        The function calculates whether the ray should reflect or refract based on the IOR and the angle of incidence.
     * @param ray_in The incoming ray.
     * @param rec The hit record containing information about the intersection point.
     * @param scatter_rec A reference to a ScatterRecord that will store the result of the scattering event.
     * @return True if the scattering (reflection or refraction) was successful, false otherwise.
     */
    bool scatter(const Ray &ray_in,
                 const HitRecord &rec,
                 ScatterRecord &scatter_rec) const override;

    /**
     * @brief Calculates the Bidirectional Reflectance Distribution Function (BRDF) for the dielectric material.
     *        This function models how light is reflected and refracted at the surface of the material.
     * @param rec The hit record containing information about the intersection.
     * @param view_dir The direction from the viewpoint (camera).
     * @param light_dir The direction from the light source.
     * @return The BRDF value representing how the material interacts with light.
     */
    Vec3 brdf(const HitRecord &rec,
              const Vec3 &view_dir,
              const Vec3 &light_dir) const override;

private:
    float m_ior; ///< The index of refraction (IOR) for the dielectric material.
};

#endif // DIELECTRIC_H_
