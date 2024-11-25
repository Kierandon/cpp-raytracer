#ifndef EMISSIVE_H
#define EMISSIVE_H

#include "materials/Material.h"

/**
 * @class Emissive
 * @brief Represents an emissive material, which emits light based on its color and intensity.
 *        This material does not scatter light like diffuse materials but instead radiates energy,
 *        simulating light sources such as lamps or screens.
 */
class Emissive : public Material
{
public:
    /**
     * @brief Constructs an Emissive material with a specified color and intensity.
     * @param color The color of the emitted light.
     * @param intensity The intensity (brightness) of the emitted light.
     */
    Emissive(const Vec3 &color, float intensity)
        : m_color(color), m_intensity(intensity) {}

    /**
     * @brief The scatter function for an emissive material does nothing, as it does not scatter light.
     *        The function simply returns false, indicating no scattering event occurs.
     * @param ray_in The incoming ray.
     * @param rec The hit record containing information about the intersection point.
     * @param scatter_rec A reference to a ScatterRecord that would store the result of scattering (not used for emissive materials).
     * @return Always returns false, as emissive materials do not scatter light.
     */
    bool scatter(const Ray &ray_in,
                 const HitRecord &rec,
                 ScatterRecord &scatter_rec) const override;

    /**
     * @brief The BRDF for an emissive material is undefined as it does not reflect light.
     *        This function will return a zero vector, as there is no diffuse or specular reflection.
     * @param rec The hit record containing information about the intersection.
     * @param view_dir The direction from the viewpoint (camera).
     * @param light_dir The direction from the light source.
     * @return Always returns a zero vector, as there is no reflection.
     */
    Vec3 brdf(const HitRecord &rec,
              const Vec3 &view_dir,
              const Vec3 &light_dir) const override;

    /**
     * @brief Calculates the emitted light from the material, which is based on the color and intensity.
     *        This function returns the color of the material multiplied by its intensity, representing
     *        the emitted radiance.
     * @param rec The hit record containing information about the intersection.
     * @param u The u-coordinate of the surface (typically used for texture mapping, not used here).
     * @param v The v-coordinate of the surface (typically used for texture mapping, not used here).
     * @param p The point of intersection on the surface.
     * @return The color of the emitted light based on the material's properties.
     */
    Vec3 emitted(const HitRecord &rec,
                 float u, float v,
                 const Vec3 &p) const override;

private:
    Vec3 m_color;      ///< The color of the emitted light.
    float m_intensity; ///< The intensity (brightness) of the emitted light.
};

#endif // EMISSIVE_H
