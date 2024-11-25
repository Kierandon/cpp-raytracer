#include "core/PhongPathtracer.h"
#include "materials/Material.h"
#include "scene/SceneConfig.h"
#include "core/Vec3.h"

#include <cmath>
#include <cstdlib>

PhongPathtracer::PhongPathtracer(int max_depth) : max_depth(max_depth) {}

// Path tracing function
Vec3 PhongPathtracer::trace(const Ray &ray, const Hittable &scene, int depth, SceneConfig &config, const std::vector<std::shared_ptr<Light>> lights) const
{
    // Base case: if maximum depth is reached, terminate
    if (depth <= 0)
    {
        return Vec3(0, 0, 0); // No contribution
    }

    HitRecord rec;
    if (scene.hit(ray, 0.001f, FLT_MAX, rec))
    {
        Vec3 scatter_direction;
        float pdf;

        // Scatter the ray using the material's properties
        Vec3 attenuation = rec.material_ptr->scatter(ray, rec, scatter_direction, pdf);

        // Shadow Ray Contribution
        Vec3 direct_lighting(0, 0, 0);
        for (const auto &light_ptr : lights)
        {
            if (light_ptr)
            {
                const Light &light = *light_ptr;
                Vec3 light_dir = (light.position - rec.point).normalized();
                float light_distance = (light.position - rec.point).length();

                Ray shadow_ray(rec.point + rec.normal * 1e-4f, light_dir);
                HitRecord shadow_rec;

                if (!scene.hit(shadow_ray, 0.001f, light_distance, shadow_rec))
                {
                    float light_intensity = std::max(0.0f, light_dir.dot(rec.normal));
                    float distance_factor = 1.0f / (light_distance * light_distance);              // Quadratic falloff
                    direct_lighting += light_intensity * light.intensity * distance_factor * 0.5f; // Scaled lighting
                }
            }
        }

        // Check if scattering occurred
        Vec3 indirect_lighting(0, 0, 0);
        if (pdf > 0)
        {
            Ray scattered(rec.point + rec.normal * 1e-4f, scatter_direction);
            // Recursive trace with attenuation and PDF
            indirect_lighting = attenuation * trace(scattered, scene, depth - 1, config, lights) / pdf;
        }

        // Combine direct and indirect lighting
        return direct_lighting + indirect_lighting;
    }
    else
    {
        return config.background_bottom;
    }
}
