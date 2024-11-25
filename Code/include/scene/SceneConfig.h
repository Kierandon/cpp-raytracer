#ifndef SCENE_CONFIG_H
#define SCENE_CONFIG_H

#include "core/Vec3.h"

/**
 * @enum RenderMode
 * @brief An enumeration of different rendering modes supported by the renderer.
 *       The render mode determines the algorithm used to render the scene, such as Path Tracing or Phong shading.
 */

enum class RenderMode
{
    BINARY,
    PHONG,
    PHONGPATH,
    PATH,
};

/**
 * @struct SceneConfig
 * @brief A structure to hold configuration settings for rendering a scene.
 *        This includes settings for the render mode, image properties, denoising, sampling, background, camera, and more.
 *        The `SceneConfig` struct provides all the necessary parameters to control the rendering process and scene settings.
 */
struct SceneConfig
{
    // Render mode
    /**
     * @brief The rendering mode to be used in the scene.
     *        Determines how the scene will be rendered, with different algorithms such as Path Tracing or Phong shading.
     */
    RenderMode render_mode = RenderMode::PATH;

    // Image settings
    /**
     * @brief The width of the rendered image.
     */
    int image_width = 1920;
    /**
     * @brief The height of the rendered image.
     */
    int image_height = 1080;
    /**
     * @brief Whether tone mapping should be applied to the final image.
     */
    bool use_tone_mapping = false;

    // Denoiser settings
    /**
     * @brief Whether to apply a denoiser to the rendered image.
     */
    bool use_denoiser = false;
    /**
     * @brief The spatial sigma for the bilateral filter denoiser, controlling how much neighboring pixels are considered.
     */
    float bilateral_sigma_spatial = 10.0f;
    /**
     * @brief The range sigma for the bilateral filter denoiser, controlling how much color similarity is considered.
     */
    float bilateral_sigma_range = 0.1f;

    // Gaussian blur settings
    /**
     * @brief Whether to apply Gaussian blur to the image.
     */
    bool use_gaussian_blur = false;
    /**
     * @brief The sigma (standard deviation) for the Gaussian blur, controlling the blur strength.
     */
    float blur_sigma = 1.0f;
    /**
     * @brief The kernel size for the Gaussian blur.
     */
    int blur_kernel_size = 5;

    // Importance sampling settings
    /**
     * @brief Whether to use importance sampling in the rendering process.
     */
    bool use_importance_sampling{false};
    /**
     * @brief The minimum number of samples to use in importance sampling.
     */
    int min_samples{4};
    /**
     * @brief The maximum number of samples to use in importance sampling.
     */
    int max_samples{64};
    /**
     * @brief The threshold for determining importance in sampling.
     */
    float importance_threshold{0.1f};

    // Background settings
    /**
     * @brief The color at the bottom of the gradient background.
     */
    Vec3 background_bottom{0.25, 0.25, 0.25};
    /**
     * @brief The color at the top of the gradient background.
     */
    Vec3 background_top{1.0, 0.0, 0.0};
    /**
     * @brief Whether to use a gradient background.
     */
    bool use_gradient = false;

    // Pathtracer settings
    /**
     * @brief Whether to use stratified sampling in the path tracer.
     */
    bool use_stratified_sampling = false;
    /**
     * @brief These variables will be populated at runtime for the path tracer's sampling settings.
     */
    int sqrt_samples = 0;
    float inv_sqrt_samples = 0.0f;
    float sqrt_samples_squared = 0.0f;

    // Render settings
    /**
     * @brief Whether to use shadow rays for shadow computation.
     */
    bool use_shadow_rays = true;
    /**
     * @brief Whether to use Bounding Volume Hierarchy (BVH) for optimized scene traversal.
     */
    bool use_bvh = true;
    /**
     * @brief The number of samples per pixel to be used during rendering.
     */
    int samples_per_pixel = 10;
    /**
     * @brief The maximum recursion depth for rays.
     */
    int max_ray_depth = 10;

    // Camera settings
    /**
     * @brief The position of the camera in the scene.
     */
    Vec3 camera_position{0, 1, -3};
    /**
     * @brief The point in the scene that the camera is looking at.
     */
    Vec3 camera_lookat{0, 0.5, 0};
    /**
     * @brief The up direction for the camera (used for orientation).
     */
    Vec3 camera_up{0, 1, 0};
    /**
     * @brief The field of view (FOV) of the camera.
     */
    float fov = 45.0f;

    // Depth of field settings
    /**
     * @brief Whether to apply depth of field (DOF) effects to the scene.
     */
    bool use_dof = false;
    /**
     * @brief The aperture size, affecting the depth of field effect.
     */
    float aperture = 0.7f;
    /**
     * @brief The focus distance for depth of field, controlling the distance at which objects appear sharp.
     */
    float focus_distance = 6.0f;
};

#endif // SCENE_CONFIG_H
