#ifndef BLINN_PHONG_MATERIAL_H
#define BLINN_PHONG_MATERIAL_H

#include "Material.h"
#include "core/Vec3.h"
#include "textures/Texture.h"
#include "lighting/Light.h"
#include "lighting/HitRecord.h"
#include "geometry/Hittable.h"
#include "core/Utils.h"
#include "scene/SceneConfig.h"

#include <vector>
#include <memory>
#define _USE_MATH_DEFINES
#include <cmath>

/**
 * @class BlinnPhongMaterial
 * @brief Represents a Blinn-Phong material model, which includes diffuse, specular, and ambient lighting calculations,
 *        as well as reflection and refraction support for more advanced shading.
 *        The material can be defined using a solid color or a texture for the diffuse component.
 */
class BlinnPhongMaterial : public Material
{
public:
    /**
     * @brief Constructs a BlinnPhongMaterial with a solid color for diffuse shading.
     * @param diffuse_color The diffuse color of the material.
     * @param specular_color The specular color of the material.
     * @param shininess The shininess coefficient, affecting the specular reflection.
     * @param kd The diffuse reflection coefficient (default: 0.9).
     * @param ks The specular reflection coefficient (default: 0.1).
     * @param ka The ambient reflection coefficient (default: 0.1).
     * @param is_reflective Whether the material is reflective (default: false).
     * @param reflectivity The reflectivity coefficient (default: 0.0).
     * @param is_refractive Whether the material is refractive (default: false).
     * @param ior The index of refraction (default: 1.0).
     */
    BlinnPhongMaterial(
        const Vec3 &diffuse_color,
        const Vec3 &specular_color,
        float shininess,
        float kd = 0.9f,
        float ks = 0.1f,
        float ka = 0.1f,
        bool is_reflective = false,
        float reflectivity = 0.0f,
        bool is_refractive = false,
        float ior = 1.0f)
        : diffuse_color(diffuse_color),
          specular_color(specular_color),
          shininess(shininess),
          kd(kd),
          ks(ks),
          ka(ka),
          is_reflective(is_reflective),
          reflectivity(reflectivity),
          is_refractive(is_refractive),
          ior(ior),
          use_texture(false)
    {
    }

    /**
     * @brief Constructs a BlinnPhongMaterial with a texture for diffuse shading.
     * @param diffuse_texture The texture for the diffuse component of the material.
     * @param specular_color The specular color of the material.
     * @param shininess The shininess coefficient, affecting the specular reflection.
     * @param kd The diffuse reflection coefficient (default: 0.9).
     * @param ks The specular reflection coefficient (default: 0.1).
     * @param ka The ambient reflection coefficient (default: 0.1).
     * @param is_reflective Whether the material is reflective (default: false).
     * @param reflectivity The reflectivity coefficient (default: 0.0).
     * @param is_refractive Whether the material is refractive (default: false).
     * @param ior The index of refraction (default: 1.0).
     */
    BlinnPhongMaterial(
        std::shared_ptr<Texture> diffuse_texture,
        const Vec3 &specular_color,
        float shininess,
        float kd = 0.9f,
        float ks = 0.1f,
        float ka = 0.1f,
        bool is_reflective = false,
        float reflectivity = 0.0f,
        bool is_refractive = false,
        float ior = 1.0f)
        : diffuse_texture(diffuse_texture),
          specular_color(specular_color),
          shininess(shininess),
          kd(kd),
          ks(ks),
          ka(ka),
          is_reflective(is_reflective),
          reflectivity(reflectivity),
          is_refractive(is_refractive),
          ior(ior),
          use_texture(true)
    {
    }

    /**
     * @brief Shades a point on the surface, calculating the color based on the material's properties and the scene's lights.
     * @param rec The hit record containing details about the intersection.
     * @param view_dir The direction from the viewpoint (camera).
     * @param lights A list of lights in the scene.
     * @param scene The scene geometry, used to check for shadows.
     * @param depth The current recursion depth for reflections/refractions.
     * @param config The scene configuration containing additional parameters.
     * @return The color resulting from the shading calculation.
     */
    virtual Vec3 shade(
        const HitRecord &rec,
        const Vec3 &view_dir,
        const std::vector<std::shared_ptr<Light>> &lights,
        const Hittable &scene,
        int depth,
        const SceneConfig &config) const override;

    /**
     * @brief Calculates the scattering of a ray, handling reflection and refraction.
     * @param ray_in The incoming ray.
     * @param rec The hit record containing details about the intersection.
     * @param scatter_direction The direction of the scattered ray.
     * @param pdf The probability density function for the scattering event.
     * @return The color of the scattered ray.
     */
    virtual Vec3 scatter(
        const Ray &ray_in,
        const HitRecord &rec,
        Vec3 &scatter_direction,
        float &pdf) const override;

private:
    /**
     * @brief Calculates the ambient lighting contribution for the material.
     * @param rec The hit record containing details about the intersection.
     * @return The ambient lighting color.
     */
    Vec3 calculateAmbient(const HitRecord &rec) const;

    /**
     * @brief Calculates the diffuse and specular lighting contributions for the material.
     * @param rec The hit record containing details about the intersection.
     * @param view_dir The view direction.
     * @param lights A list of lights in the scene.
     * @param scene The scene geometry.
     * @param config The scene configuration.
     * @return The combined diffuse and specular color.
     */
    Vec3 calculateDiffuseAndSpecular(
        const HitRecord &rec,
        const Vec3 &view_dir,
        const std::vector<std::shared_ptr<Light>> &lights,
        const Hittable &scene,
        const SceneConfig &config) const;

    /**
     * @brief Checks if a point is in shadow from a light source.
     * @param rec The hit record.
     * @param light_dir The direction to the light.
     * @param scene The scene geometry.
     * @param light_position The position of the light.
     * @return True if the point is in shadow, false otherwise.
     */
    bool isInShadow(
        const HitRecord &rec,
        const Vec3 &light_dir,
        const Hittable &scene,
        const Vec3 &light_position) const;

    /**
     * @brief Calculates the reflection of light from the material.
     * @param rec The hit record.
     * @param view_dir The view direction.
     * @param lights A list of lights in the scene.
     * @param scene The scene geometry.
     * @param depth The recursion depth for reflection.
     * @param config The scene configuration.
     * @return The color of the reflected ray.
     */
    Vec3 calculateReflection(
        const HitRecord &rec,
        const Vec3 &view_dir,
        const std::vector<std::shared_ptr<Light>> &lights,
        const Hittable &scene,
        int depth,
        const SceneConfig &config) const;

    /**
     * @brief Calculates the refraction of light through the material.
     * @param rec The hit record.
     * @param view_dir The view direction.
     * @param lights A list of lights in the scene.
     * @param scene The scene geometry.
     * @param depth The recursion depth for refraction.
     * @param config The scene configuration.
     * @return The color of the refracted ray.
     */
    Vec3 calculateRefraction(
        const HitRecord &rec,
        const Vec3 &view_dir,
        const std::vector<std::shared_ptr<Light>> &lights,
        const Hittable &scene,
        int depth,
        const SceneConfig &config) const;

    /**
     * @brief Calculates the background color for rays that miss the scene.
     * @param direction The direction of the ray.
     * @param config The scene configuration.
     * @return The background color.
     */
    Vec3 calculateBackgroundColor(const Vec3 &direction, const SceneConfig &config) const;

    Vec3 diffuse_color;                       ///< The solid color used for diffuse shading.
    std::shared_ptr<Texture> diffuse_texture; ///< The texture used for diffuse shading.
    Vec3 specular_color;                      ///< The color used for specular highlights.
    float shininess;                          ///< The shininess coefficient, affecting specular reflections.
    float kd, ks, ka;                         ///< The diffuse, specular, and ambient reflection coefficients.
    bool is_reflective;                       ///< Whether the material is reflective.
    float reflectivity;                       ///< The reflectivity coefficient.
    bool is_refractive;                       ///< Whether the material is refractive.
    float ior;                                ///< The index of refraction for the material.
    bool use_texture;                         ///< Whether a texture is used for diffuse shading.
};

#endif // BLINN_PHONG_MATERIAL_H
