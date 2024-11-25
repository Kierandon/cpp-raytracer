#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "core/Camera.h"
#include "core/Image.h"
#include "geometry/HittableList.h"
#include "lighting/Light.h"
#include "scene/Scene.h"
#include "scene/SceneConfig.h"
#include "materials/Material.h"
#include "core/Pathtracer.h"

#include <nlohmann/json.hpp>

#include <memory>
#include <vector>
#include <mutex>

/**
 * @class SceneRenderer
 * @brief Responsible for rendering a scene based on a given configuration.
 *        The `SceneRenderer` class handles the different rendering modes, updates progress, and samples pixels
 *        for path tracing or shading algorithms. It manages scene components and ensures the proper rendering output.
 */
class SceneRenderer
{
public:
    /**
     * @brief Renders a scene and saves the output to a specified file path.
     * @param scene The scene to be rendered.
     * @param config The configuration settings for rendering, such as camera settings, render mode, etc.
     * @param output_path The path where the rendered image will be saved.
     */
    void render(Scene &scene, SceneConfig &config, std::string &output_path);

private:
    /**
     * @brief Renders the scene using the Phong or Binary rendering mode.
     * @param scene The scene to be rendered.
     * @param config The configuration settings for rendering.
     * @param pixels_done A reference to an atomic integer tracking the number of completed pixels.
     * @param total_pixels The total number of pixels to be rendered.
     */
    void render_phong_or_binary(Scene &scene, SceneConfig &config, std::atomic<int> &pixels_done, int total_pixels);

    /**
     * @brief Renders the scene using the Phong Path tracing mode.
     * @param scene The scene to be rendered.
     * @param config The configuration settings for rendering.
     * @param pixels_done A reference to an atomic integer tracking the number of completed pixels.
     * @param total_pixels The total number of pixels to be rendered.
     */
    void render_phongpath(Scene &scene, SceneConfig &config, std::atomic<int> &pixels_done, int total_pixels);

    /**
     * @brief Renders the scene using Path tracing mode.
     * @param scene The scene to be rendered.
     * @param config The configuration settings for rendering.
     * @param pixels_done A reference to an atomic integer tracking the number of completed pixels.
     * @param total_pixels The total number of pixels to be rendered.
     */
    void render_path(Scene &scene, SceneConfig &config, std::atomic<int> &pixels_done, int total_pixels);

    /**
     * @brief Updates the progress of the rendering process and displays it to the console.
     * @param pixels_done A reference to an atomic integer tracking the number of completed pixels.
     * @param total_pixels The total number of pixels to be rendered.
     */
    void update_progress(std::atomic<int> &pixels_done, int total_pixels);

    /**
     * @brief Samples a pixel during the rendering process.
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     * @param num_samples The number of samples to take for the pixel.
     * @param stratified_sampling Whether stratified sampling is enabled.
     * @param scene The scene to be rendered.
     * @param config The configuration settings for rendering.
     * @param path_tracer The path tracing object used for sampling.
     * @return The color value of the sampled pixel.
     */
    Vec3 sample_pixel(int x, int y, int num_samples, bool stratified_sampling, Scene &scene, SceneConfig &config, Pathtracer &path_tracer);

    // Scene components
    std::mutex console_mutex; ///< Mutex to protect the console output from concurrent access.

    // Helper methods
    /**
     * @brief Creates a material based on the specified type.
     * @param type The type of the material to be created (e.g., "lambertian", "metal").
     * @return A shared pointer to the created material.
     */
    std::shared_ptr<Material> create_material(const std::string &type);

    /**
     * @brief Prints the configuration settings to the console for debugging or information purposes.
     * @param config The scene configuration to be printed.
     */
    void print_config(const SceneConfig &config);

    /**
     * @brief Computes the background color based on the configuration and the given ray.
     * @param config The scene configuration, which may include background settings.
     * @param ray The ray used to compute the background color (for skyboxes, gradients, etc.).
     * @return The background color corresponding to the ray.
     */
    Vec3 compute_background_color(SceneConfig &config, const Ray &ray) const;
};

#endif // SCENE_MANAGER_H
