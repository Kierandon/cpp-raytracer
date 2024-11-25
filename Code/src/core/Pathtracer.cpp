#include "core/Pathtracer.h"

Vec3 Pathtracer::trace(const Ray &ray, const Hittable &world, int depth, const std::vector<std::shared_ptr<Light>> &lights)
{
    if (depth <= 0)
        return Vec3(0, 0, 0);

    HitRecord rec;
    if (!world.hit(ray, 0.001f, std::numeric_limits<float>::infinity(), rec))
    {
        return background_color(ray);
    }

    ScatterRecord scatter_rec;
    Vec3 emitted = rec.material_ptr->emitted(rec, rec.u, rec.v, rec.point);

    if (!rec.material_ptr->scatter(ray, rec, scatter_rec))
    {
        return clamp_radiance(emitted);
    }

    // Calculate throughput probability for Russian Roulette
    Vec3 throughput = scatter_rec.attenuation;
    float max_channel = std::max(throughput.x, std::max(throughput.y, throughput.z));
    float roulette_probability = std::clamp(max_channel, 0.1f, 1.0f);

    if (depth > 2 && random_float() > roulette_probability)
    {
        return clamp_radiance(emitted);
    }

    // Account for the survival probability in the recursion
    scatter_rec.attenuation /= roulette_probability;

    Vec3 direct_lighting = Vec3(0, 0, 0);
    if (emitted.length() * emitted.length() < 0.001f)
    {
        direct_lighting = compute_direct_lighting(rec, world, ray, lights);
    }

    Vec3 indirect_lighting;
    if (scatter_rec.specular_ray)
    {
        Ray scattered(rec.point, scatter_rec.specular_direction);
        indirect_lighting = trace(scattered, world, depth - 1, lights);
        return clamp_radiance(emitted + scatter_rec.attenuation * indirect_lighting);
    }
    else if (scatter_rec.pdf_ptr)
    {
        Vec3 direction = scatter_rec.pdf_ptr->generate();
        Ray scattered(rec.point, direction);
        float pdf = scatter_rec.pdf_ptr->value(direction);

        if (pdf <= 0.0f)
            return clamp_radiance(emitted);

        indirect_lighting = trace(scattered, world, depth - 1, lights);
        Vec3 brdf = rec.material_ptr->brdf(rec, -ray.direction(), direction);
        return clamp_radiance(emitted + direct_lighting + (brdf * indirect_lighting) / pdf);
    }

    return clamp_radiance(emitted);
}

Vec3 Pathtracer::background_color(const Ray &ray)
{
    if (scene_config.render_mode == RenderMode::BINARY)
    {
        return Vec3(0.0f, 0.0f, 0.0f);
    }
    if (!scene_config.use_gradient)
    {
        return scene_config.background_bottom;
    }
    Vec3 unit_direction = ray.direction().normalized();
    float t = 0.5 * (unit_direction.y + 1.0);
    return (1.0 - t) * scene_config.background_bottom + t * scene_config.background_top;
}

Vec3 Pathtracer::compute_direct_lighting(const HitRecord &rec,
                                         const Hittable &world,
                                         const Ray &incident_ray,
                                         const std::vector<std::shared_ptr<Light>> &lights)
{
    Vec3 direct_light(0, 0, 0);

    for (const auto &light : lights)
    {
        Vec3 light_dir, light_intensity;
        float distance;

        // Get light sample
        light->sample(rec.point, light_dir, light_intensity, distance);

        // Ensure light direction is normalized
        light_dir = light_dir.normalized();

        // Shadow ray
        Ray shadow_ray(rec.point, light_dir);
        HitRecord shadow_rec;

        // Check for shadows
        if (!world.hit(shadow_ray, 0.001f, distance - 0.001f, shadow_rec))
        {
            float cos_theta = std::max(0.0f, rec.normal.dot(light_dir));

            // Skip if surface is facing away from light
            if (cos_theta > 0.0f)
            {
                float distance_squared = distance * distance;
                Vec3 attenuated_intensity = light_intensity / distance_squared;

                Vec3 brdf = rec.material_ptr->brdf(rec, -incident_ray.direction(), light_dir);
                direct_light += brdf * attenuated_intensity * cos_theta;
            }
        }
    }

    return direct_light;
}

Vec3 Pathtracer::clamp_radiance(const Vec3 &v, float max_value)
{
    return Vec3(
        std::min(v.x, max_value),
        std::min(v.y, max_value),
        std::min(v.z, max_value));
}
