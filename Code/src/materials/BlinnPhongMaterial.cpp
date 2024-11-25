#include "materials/BlinnPhongMaterial.h"

#include <algorithm>
#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>

Vec3 random_unit_vector();

Vec3 BlinnPhongMaterial::shade(
    const HitRecord &rec,
    const Vec3 &view_dir,
    const std::vector<std::shared_ptr<Light>> &lights,
    const Hittable &scene,
    int depth,
    const SceneConfig &config) const
{
    Vec3 final_color(0.0f, 0.0f, 0.0f);

    if (!is_reflective || reflectivity < 1.0f)
    {
        // Ambient contribution
        Vec3 ambient = calculateAmbient(rec);
        final_color += ambient;

        // Diffuse and specular contributions
        Vec3 diffuse_specular = calculateDiffuseAndSpecular(rec, view_dir, lights, scene, config);
        final_color += diffuse_specular;
    }

    // Reflective term
    if (is_reflective && depth > 0)
    {
        Vec3 reflection = calculateReflection(rec, view_dir, lights, scene, depth, config);
        final_color = reflectivity * reflection + (1.0f - reflectivity) * final_color;
    }

    // Refractive term
    if (is_refractive && depth > 0)
    {
        Vec3 refraction = calculateRefraction(rec, view_dir, lights, scene, depth, config);
        final_color = (1.0f - reflectivity) * refraction;
    }

    return final_color;
}

Vec3 BlinnPhongMaterial::calculateAmbient(const HitRecord &rec) const
{
    Vec3 ambient_color = use_texture ? diffuse_texture->value(rec.u, rec.v, rec.point) : diffuse_color;
    return ka * ambient_color;
}

Vec3 BlinnPhongMaterial::calculateDiffuseAndSpecular(
    const HitRecord &rec,
    const Vec3 &view_dir,
    const std::vector<std::shared_ptr<Light>> &lights,
    const Hittable &scene,
    const SceneConfig &config) const
{
    Vec3 result(0.0f, 0.0f, 0.0f);

    for (const auto &light : lights)
    {
        Vec3 light_dir = (light->position - rec.point).normalized();
        Vec3 light_intensity = light->intensity;

        // Shadow check
        if (config.use_shadow_rays && isInShadow(rec, light_dir, scene, light->position))
        {
            continue;
        }

        // Diffuse term
        float diffuse_factor = std::max(0.0f, rec.normal.dot(light_dir));
        Vec3 diffuse_color_value = use_texture ? diffuse_texture->value(rec.u, rec.v, rec.point) : diffuse_color;
        Vec3 diffuse = kd * diffuse_factor * diffuse_color_value;

        // Specular term
        Vec3 half_vector = (view_dir + light_dir).normalized();
        float specular_factor = std::pow(std::max(0.0f, rec.normal.dot(half_vector)), shininess);
        Vec3 specular = ks * specular_factor * specular_color;

        // Accumulate contributions
        result += (diffuse + specular) * light_intensity;
    }

    return result;
}

bool BlinnPhongMaterial::isInShadow(
    const HitRecord &rec,
    const Vec3 &light_dir,
    const Hittable &scene,
    const Vec3 &light_position) const
{
    Ray shadow_ray(rec.point + rec.normal * 1e-4f, light_dir);
    HitRecord shadow_rec;
    float max_distance = (light_position - rec.point).length();

    return scene.hit(shadow_ray, 0.001f, max_distance, shadow_rec);
}

Vec3 BlinnPhongMaterial::calculateReflection(
    const HitRecord &rec,
    const Vec3 &view_dir,
    const std::vector<std::shared_ptr<Light>> &lights,
    const Hittable &scene,
    int depth,
    const SceneConfig &config) const
{
    Vec3 reflect_dir = reflect(-view_dir, rec.normal).normalized();
    Ray reflect_ray(rec.point + rec.normal * 1e-4f, reflect_dir);
    HitRecord reflection_rec;

    if (scene.hit(reflect_ray, 0.001f, FLT_MAX, reflection_rec))
    {
        Vec3 reflected_view_dir = -reflect_ray.direction().normalized();
        return reflection_rec.material_ptr->shade(
            reflection_rec, reflected_view_dir, lights, scene, depth - 1, config);
    }
    else
    {
        return calculateBackgroundColor(reflect_ray.direction(), config);
    }
}

Vec3 BlinnPhongMaterial::calculateRefraction(
    const HitRecord &rec,
    const Vec3 &view_dir,
    const std::vector<std::shared_ptr<Light>> &lights,
    const Hittable &scene,
    int depth,
    const SceneConfig &config) const
{
    const float EPSILON = 1e-4f;
    const float MIN_HIT_DISTANCE = 0.001f;

    float eta = rec.front_face ? (1.0f / ior) : ior;
    Vec3 refract_dir = refract(-view_dir, rec.normal, eta).normalized();
    Ray refract_ray(rec.point - rec.normal * EPSILON, refract_dir);
    HitRecord refraction_rec;

    float cosTheta = std::min(view_dir.dot(rec.normal), 1.0f);
    float sinTheta = std::sqrt(1.0f - cosTheta * cosTheta);

    // Check for total internal reflection
    bool cannot_refract = eta * sinTheta > 1.0f;
    float fresnel = schlick(cosTheta, eta);

    Vec3 refraction_color;
    if (cannot_refract || scene.hit(refract_ray, MIN_HIT_DISTANCE, FLT_MAX, refraction_rec))
    {
        if (cannot_refract)
        {
            // Total internal reflection
            Vec3 reflection_color = calculateReflection(rec, view_dir, lights, scene, depth, config);
            return reflection_color;
        }
        else
        {
            Vec3 refracted_view_dir = -refract_ray.direction().normalized();
            refraction_color = refraction_rec.material_ptr->shade(
                refraction_rec, refracted_view_dir, lights, scene, depth - 1, config);
        }
    }
    else
    {
        refraction_color = calculateBackgroundColor(refract_ray.direction(), config);
    }

    Vec3 reflection_color = calculateReflection(rec, view_dir, lights, scene, depth, config);

    return fresnel * reflection_color + (1.0f - fresnel) * refraction_color;
}

Vec3 BlinnPhongMaterial::calculateBackgroundColor(const Vec3 &direction, const SceneConfig &config) const
{
    Vec3 unit_direction = direction.normalized();
    float t = 0.5f * (unit_direction.y + 1.0f);

    if (config.use_gradient)
    {
        return (1.0f - t) * config.background_bottom + t * config.background_top;
    }
    else
    {
        return config.background_bottom;
    }
}


Vec3 BlinnPhongMaterial::scatter(
    const Ray &ray_in,
    const HitRecord &rec,
    Vec3 &scatter_direction,
    float &pdf) const
{
    if (is_reflective)
    {
        scatter_direction = reflect(ray_in.direction().normalized(), rec.normal).normalized();
        pdf = 1.0f; // Perfect reflection has a delta PDF
        return reflectivity * specular_color;
    }

    if (is_refractive)
    {
        // Compute the refractive index ratio
        float eta = rec.front_face ? (1.0f / ior) : ior;

        // Compute the refracted direction
        scatter_direction = refract(ray_in.direction().normalized(), rec.normal, eta).normalized();

        // Compute the cosine term (angle between refracted direction and normal)
        float cos_theta = fabs(scatter_direction.dot(rec.normal));

        // PDF is 1 because this is a delta distribution
        pdf = 1.0f;

        // Compute the BTDF value, scaled by the cosine term and eta^2
        float eta_scale = rec.front_face ? (1.0f / (eta * eta)) : (eta * eta);
        Vec3 btdf = (1.0f - reflectivity) * specular_color * cos_theta * eta_scale;

        // Return the BTDF value
        if (btdf.length() < 1e-6f)
        {
            std::cerr << "Black pixel at BTDF: " << btdf << std::endl;
        }

        return btdf;
    }

    scatter_direction = rec.normal + random_unit_vector(); // Random diffuse bounce

    // Handle degenerate scatter direction
    if (scatter_direction.near_zero())
        scatter_direction = rec.normal;

    // Compute the probability density function (PDF)
    pdf = rec.normal.dot(scatter_direction.normalized()) / M_PI;

    // Compute the half-vector
    Vec3 view_dir = -ray_in.direction().normalized();
    Vec3 light_dir = scatter_direction.normalized();
    Vec3 half_vector = (view_dir + light_dir).normalized();

    // Compute the specular component
    float spec_angle = std::max(rec.normal.dot(half_vector), 0.0f);
    Vec3 specular = ks * ((shininess + 2.0f) / (2.0f * M_PI)) * pow(spec_angle, shininess) * specular_color;

    // Compute the diffuse component
    Vec3 diffuse = kd * (use_texture ? diffuse_texture->value(rec.u, rec.v, rec.point) : diffuse_color) / M_PI;

    // Return the combined BRDF value
    return diffuse + specular;
}
