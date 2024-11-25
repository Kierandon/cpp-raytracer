#include "scene/SceneRenderer.h"
#include "core/Utils.h"
#include "materials/BinaryMaterial.h"
#include "materials/BlinnPhongMaterial.h"
#include "textures/CheckerTexture.h"
#include "textures/ImageTexture.h"
#include "core/ImportanceSampler.h"
#include "textures/SolidColor.h"
#include "geometry/Sphere.h"
#include "geometry/Plane.h"
#include "geometry/Triangle.h"
#include "geometry/Cylinder.h"
#include "geometry/Rectangle.h"
#include "geometry/BVHNode.h"
#include "core/PhongPathtracer.h"
#include "core/Pathtracer.h"
#include "postprocess/ReinhardToneMapper.h"

#include <nlohmann/json.hpp>
#include <fstream>
#include <cfloat>
#include <chrono>
#include <iostream>
#include <atomic>

void SceneRenderer::render(Scene &scene, SceneConfig &config, std::string &output_path)
{
    std::cout << "Starting render... "
              << (config.use_bvh ? "(Using BVH)" : "(No BVH)")
              << " (Samples per pixel: " << config.samples_per_pixel << ")"
              << " (Max ray depth: " << config.max_ray_depth << ")"
              << " (Resolution: " << config.image_width << "x" << config.image_height << ")"
              << std::endl;

    auto start_time = std::chrono::high_resolution_clock::now();

    std::atomic<int> pixels_done = 0;
    int total_pixels = config.image_width * config.image_height;

    // Select render function based on mode
    if (config.render_mode == RenderMode::PHONG || config.render_mode == RenderMode::BINARY)
    {
        render_phong_or_binary(scene, config, pixels_done, total_pixels);
    }
    else if (config.render_mode == RenderMode::PHONGPATH)
    {
        render_phongpath(scene, config, pixels_done, total_pixels);
    }
    else if (config.render_mode == RenderMode::PATH)
    {
        render_path(scene, config, pixels_done, total_pixels);
    }
    else
    {
        throw std::runtime_error("Unsupported render mode!");
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    float seconds = duration.count() / 1000.0f;
    float avg_ms_per_pixel = duration.count() / float(total_pixels);

    std::cout << "\nRender complete!" << std::endl;
    std::cout << "Total time: " << seconds << " seconds" << std::endl;
    std::cout << "Average time per pixel: " << avg_ms_per_pixel << " ms" << std::endl;
    std::cout << "Pixels per second: " << total_pixels / seconds << std::endl;

    scene.image->save_ppm(output_path);
}

void SceneRenderer::render_path(Scene &scene, SceneConfig &config, std::atomic<int> &pixels_done, int total_pixels)
{
    Pathtracer path_tracer(config);
    std::unique_ptr<ImportanceSampler> importance_sampler;
    if (config.use_importance_sampling)
    {
        importance_sampler = std::make_unique<ImportanceSampler>(config);
        std::cout << "Rendering using path tracing with importance sampling..." << std::endl;
    }
    else
    {
        std::cout << "Rendering using path tracing..." << std::endl;
    }

#pragma omp parallel for schedule(dynamic)
    for (int y = 0; y < config.image_height; ++y)
    {
        for (int x = 0; x < config.image_width; ++x)
        {
            Vec3 pixel_color(0, 0, 0);
            float num_samples;

            if (config.use_importance_sampling)
            {
                // Get initial sample for importance
                float u = (float(x) + random_float()) / (config.image_width - 1);
                float v = (float(y) + random_float()) / (config.image_height - 1);
                Ray r = scene.camera->get_ray(u, v);
                Vec3 first_sample = path_tracer.trace(r, *scene.scene_root, config.max_ray_depth, scene.lights);
                pixel_color = first_sample;

                float importance = importance_sampler->calculate_importance(first_sample);
                num_samples = std::max(
                    float(config.min_samples),
                    std::min(importance * config.samples_per_pixel, float(config.max_samples)));

                // Additional samples
                for (int s = 1; s < static_cast<int>(num_samples); ++s)
                {
                    float u = (float(x) + random_float()) / (config.image_width - 1);
                    float v = (float(y) + random_float()) / (config.image_height - 1);
                    Ray r = scene.camera->get_ray(u, v);
                    pixel_color += path_tracer.trace(r, *scene.scene_root, config.max_ray_depth, scene.lights);
                }
            }
            else
            {
                num_samples = config.use_stratified_sampling ? config.sqrt_samples_squared : config.samples_per_pixel;

                if (config.use_stratified_sampling)
                {
                    for (int sy = 0; sy < config.sqrt_samples; ++sy)
                    {
                        for (int sx = 0; sx < config.sqrt_samples; ++sx)
                        {
                            float r1 = random_float() * config.inv_sqrt_samples;
                            float r2 = random_float() * config.inv_sqrt_samples;

                            float u = (float(x) + (sx * config.inv_sqrt_samples + r1)) / (config.image_width - 1);
                            float v = (float(y) + (sy * config.inv_sqrt_samples + r2)) / (config.image_height - 1);
                            Ray r = scene.camera->get_ray(u, v);
                            pixel_color += path_tracer.trace(r, *scene.scene_root, config.max_ray_depth, scene.lights);
                        }
                    }
                }
                else
                {
                    for (int s = 0; s < config.samples_per_pixel; ++s)
                    {
                        float u = (float(x) + random_float()) / (config.image_width - 1);
                        float v = (float(y) + random_float()) / (config.image_height - 1);
                        Ray r = scene.camera->get_ray(u, v);
                        pixel_color += path_tracer.trace(r, *scene.scene_root, config.max_ray_depth, scene.lights);
                    }
                }
            }

            pixel_color /= num_samples;
            scene.image->set_pixel(x, y, pixel_color);
            update_progress(pixels_done, total_pixels);
        }
    }
}

void SceneRenderer::render_phong_or_binary(Scene &scene, SceneConfig &config, std::atomic<int> &pixels_done, int total_pixels)
{
    // #pragma omp parallel for schedule(dynamic)
    for (int y = 0; y < config.image_height; ++y)
    {
        for (int x = 0; x < config.image_width; ++x)
        {
            Vec3 pixel_color(0, 0, 0);
            for (int s = 0; s < config.samples_per_pixel; ++s)
            {
                float u = (float(x) + random_float()) / (config.image_width - 1);
                float v = (float(y) + random_float()) / (config.image_height - 1);
                Ray r = scene.camera->get_ray(u, v);

                HitRecord rec;
                if (scene.scene_root->hit(r, 0.001, FLT_MAX, rec))
                {
                    Vec3 view_dir = -r.direction().normalized();
                    pixel_color += rec.material_ptr->shade(rec, view_dir, scene.lights, *scene.scene_root, config.max_ray_depth, config);
                }
                else
                {
                    pixel_color += compute_background_color(config, r);
                }
            }

            pixel_color /= float(config.samples_per_pixel);
            scene.image->set_pixel(x, y, pixel_color);

            update_progress(pixels_done, total_pixels);
        }
    }
}

// DO NOT UPDATE THIS FUNCTION, USE REAL PATH TRACING INSTEAD
void SceneRenderer::render_phongpath(Scene &scene, SceneConfig &config, std::atomic<int> &pixels_done, int total_pixels)
{
    PhongPathtracer path_tracer(config.max_ray_depth);
    std::cout << "Rendering using path tracing..." << std::endl;

#pragma omp parallel for schedule(dynamic)
    for (int y = 0; y < config.image_height; ++y)
    {
        for (int x = 0; x < config.image_width; ++x)
        {
            Vec3 pixel_color(0, 0, 0);

            if (config.use_stratified_sampling)
            {
                int sqrt_samples = static_cast<int>(std::sqrt(config.samples_per_pixel));
                float inv_sqrt_samples = 1.0f / sqrt_samples;

                for (int sy = 0; sy < sqrt_samples; ++sy)
                {
                    for (int sx = 0; sx < sqrt_samples; ++sx)
                    {
                        float u = (float(x) + (sx + random_float()) * inv_sqrt_samples) / (config.image_width - 1);
                        float v = (float(y) + (sy + random_float()) * inv_sqrt_samples) / (config.image_height - 1);
                        Ray r = scene.camera->get_ray(u, v);
                        pixel_color += path_tracer.trace(r, *scene.scene_root, config.max_ray_depth, config, scene.lights);
                    }
                }

                pixel_color /= float(sqrt_samples * sqrt_samples);
            }
            else
            {
                for (int s = 0; s < config.samples_per_pixel; ++s)
                {
                    float u = (float(x) + random_float()) / (config.image_width - 1);
                    float v = (float(y) + random_float()) / (config.image_height - 1);

                    Ray r = scene.camera->get_ray(u, v);
                    pixel_color += path_tracer.trace(r, *scene.scene_root, config.max_ray_depth, config, scene.lights);
                }

                pixel_color /= float(config.samples_per_pixel);
            }

            scene.image->set_pixel(x, y, pixel_color);

            update_progress(pixels_done, total_pixels);
        }
    }
}

void SceneRenderer::update_progress(std::atomic<int> &pixels_done, int total_pixels)
{
    int done = ++pixels_done;
    if (done % (total_pixels / 100) == 0) // Update every 1% of total progress
    {
        std::lock_guard<std::mutex> lock(console_mutex);
        std::cout << "\rProgress: " << (done * 100) / total_pixels << "% " << std::flush;
    }
}

Vec3 SceneRenderer::compute_background_color(SceneConfig &config, const Ray &ray) const
{
    if (config.render_mode == RenderMode::BINARY)
    {
        return Vec3(0.0f, 0.0f, 0.0f);
    }
    if (!config.use_gradient)
    {
        return config.background_bottom;
    }
    Vec3 unit_direction = ray.direction().normalized();
    float t = 0.5 * (unit_direction.y + 1.0);
    return (1.0 - t) * config.background_bottom + t * config.background_top;
}
