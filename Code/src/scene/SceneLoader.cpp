#include "scene/SceneLoader.h"
#include "materials/BlinnPhongMaterial.h"
#include "materials/BinaryMaterial.h"
#include "materials/Dielectric.h"
#include "materials/Emissive.h"
#include "materials/Metal.h"
#include "materials/Diffuse.h"
#include "textures/ImageTexture.h"
#include "geometry/Sphere.h"
#include "geometry/Cylinder.h"
#include "geometry/Triangle.h"
#include "geometry/Rectangle.h"
#include "geometry/Box.h"

void SceneLoader::load_default_scene(Scene &scene, SceneConfig &config)
{
    setup_default_scene(scene, config);

    scene.camera = std::make_shared<Camera>(
        config.camera_position,
        config.camera_lookat,
        config.camera_up,
        config.fov,
        static_cast<float>(config.image_width) / config.image_height,
        config.aperture,
        config.focus_distance,
        config.use_dof);

    scene.image = std::make_unique<Image>(config);

    build_scene(scene, config);
}

void SceneLoader::load_scene_from_json(Scene &scene, SceneConfig &config, const std::string &json_path)
{
    setup_custom_scene(scene, config, json_path);
    build_scene(scene, config);
}

void SceneLoader::build_scene(Scene &scene, SceneConfig &config)
{
    if (config.use_stratified_sampling)
    {
        config.sqrt_samples = static_cast<int>(std::sqrt(config.samples_per_pixel));
        config.inv_sqrt_samples = 1.0f / config.sqrt_samples;
        config.sqrt_samples_squared = float(config.sqrt_samples * config.sqrt_samples);
    }
    if (config.use_bvh)
    {
        scene.scene_root = std::make_shared<BVHNode>(scene.objects, 0, scene.objects.size());
    }
    else
    {
        auto list = std::make_shared<HittableList>();
        for (const auto &object : scene.objects)
        {
            list->add(object);
        }
        scene.scene_root = list;
    }
}

void SceneLoader::setup_default_scene(Scene &scene, [[maybe_unused]] SceneConfig &config)
{
    // Materials with more variety
    auto ground_diffuse = std::make_shared<Diffuse>(Vec3(0.2, 0.2, 0.2));
    auto red_metal = std::make_shared<Metal>(Vec3(0.8, 0.2, 0.2), 0.1);
    auto blue_metal = std::make_shared<Metal>(Vec3(0.2, 0.2, 0.8), 0.3);
    auto glass_sphere = std::make_shared<Dielectric>(1.5);
    auto key_light = std::make_shared<Emissive>(Vec3(1.0, 0.9, 0.8), 30.0);

    // Ground (large sphere for ground plane)
    auto ground1 = std::make_shared<Triangle>(Vec3(-3, 0, -3), Vec3(3, 0, 3), Vec3(-3, 0, 3), ground_diffuse);
    auto ground2 = std::make_shared<Triangle>(Vec3(-3, 0, -3), Vec3(3, 0, 3), Vec3(3, 0, -3), ground_diffuse);

    // // Spheres on the ground
    auto center_sphere = std::make_shared<Sphere>(Vec3(0, 0.5, -1), 0.5, key_light);
    auto left_sphere = std::make_shared<Sphere>(Vec3(-1, 0.5, -1), 0.5, red_metal);
    auto right_sphere = std::make_shared<Sphere>(Vec3(1, 0.5, -1), 0.5, blue_metal);
    auto light_sphere = std::make_shared<Sphere>(Vec3(0, 1.5, -1.5), 0.25, key_light);
    // auto box = std::make_shared<Box>(Vec3(-1, 0, -2), Vec3(1, 1, 0), red_metal);
    auto rectangle = std::make_shared<Rectangle>(Vec3(-3, 0, 3), Vec3(3, 1, -3), blue_metal);

    scene.objects.push_back(rectangle);

    scene.objects.push_back(center_sphere);
    // scene.objects.push_back(left_sphere);
    // scene.objects.push_back(right_sphere);
    // scene.objects.push_back(light_sphere);
    // scene.objects.push_back(box);
    scene.lights.push_back(std::make_shared<Light>(Vec3(0, 1, -5), Vec3(25)));
    config.camera_position = Vec3(3, 1, -4);
    config.camera_lookat = Vec3(0, 1, 0);
}

void SceneLoader::setup_custom_scene(Scene &scene, SceneConfig &config, const std::string &json_path)
{
    std::ifstream file(json_path);
    if (!file.is_open())
    {
        std::cerr << "Error: Cannot open JSON file: " << json_path << std::endl;
        return;
    }

    nlohmann::json json;
    file >> json;
    file.close();

    // Now, process the JSON data
    parse_json(scene, config, json);
}

void SceneLoader::parse_json(Scene &scene, SceneConfig &config, nlohmann::json &json)
{
    // Parse render settings
    if (json.contains("nbounces"))
    {
        config.max_ray_depth = json["nbounces"].get<int>();
    }
    if (json.contains("nsamples"))
    {
        config.samples_per_pixel = json["nsamples"].get<int>();
    }
    if (json.contains("use_stratified_sampling"))
    {
        config.use_stratified_sampling = json["use_stratified_sampling"].get<bool>();
    }
    if (json.contains("use_dof"))
    {
        config.use_dof = json["use_dof"].get<bool>();
        config.aperture = json["aperture"].get<float>();
        config.focus_distance = json["focus_distance"].get<float>();
    }
    if (json.contains("use_bvh"))
    {
        config.use_bvh = json["use_bvh"].get<bool>();
    }
    if (json.contains("use_denoiser"))
    {
        config.use_denoiser = json["use_denoiser"].get<bool>();
        if (json.contains("bilateral_sigma_spatial"))
        {
            config.bilateral_sigma_spatial = json["bilateral_sigma_spatial"].get<float>();
        }
        if (json.contains("bilateral_sigma_range"))
        {
            config.bilateral_sigma_range = json["bilateral_sigma_range"].get<float>();
        }
    }
    if (json.contains("use_shadow_rays"))
    {
        config.use_shadow_rays = json["use_shadow_rays"].get<bool>();
    }
    if (json.contains("use_tone_mapping"))
    {
        config.use_tone_mapping = json["use_tone_mapping"].get<bool>();
    }
    if (json.contains("use_gaussian_blur"))
    {
        config.use_gaussian_blur = json["use_gaussian_blur"].get<bool>();
        if (json.contains("blur_sigma"))
        {
            config.blur_sigma = json["blur_sigma"].get<float>();
        }
        if (json.contains("blur_kernel_size"))
        {
            config.blur_kernel_size = json["blur_kernel_size"].get<int>();
        }
    }
    if (json.contains("use_importance_sampling"))
    {
        config.use_importance_sampling = json["use_importance_sampling"].get<bool>();
        if (json.contains("importance_sampling_min_samples"))
        {
            config.min_samples = json["importance_sampling_min_samples"].get<int>();
        }
        if (json.contains("importance_sampling_max_samples"))
        {
            config.max_samples = json["importance_sampling_max_samples"].get<int>();
        }
        if (json.contains("importance_sampling_importance_threshold"))
        {
            config.importance_threshold = json["importance_sampling_importance_threshold"].get<float>();
        }
    }
    if (json.contains("rendermode"))
    {
        std::string mode = json["rendermode"].get<std::string>();
        if (mode == "phong")
        {
            config.render_mode = RenderMode::PHONG;
        }
        else if (mode == "phongpath")
        {
            config.render_mode = RenderMode::PHONGPATH;
        }
        else if (mode == "path")
        {
            config.render_mode = RenderMode::PATH;
        }
        else
        {
            config.render_mode = RenderMode::BINARY;
        }
    }

    // Parse camera settings
    if (json.contains("camera"))
    {
        parse_camera(scene, config, json["camera"]);
    }

    // Parse scene (background, lights, shapes)
    if (json.contains("scene"))
    {
        parse_scene(scene, config, json["scene"]);
    }
}

void SceneLoader::parse_camera(Scene &scene, SceneConfig &config, const nlohmann::json &camera_json)
{
    if (camera_json.contains("position"))
    {
        config.camera_position = Vec3(
            camera_json["position"][0].get<float>(),
            camera_json["position"][1].get<float>(),
            camera_json["position"][2].get<float>());
    }

    if (camera_json.contains("lookAt"))
    {
        config.camera_lookat = Vec3(
            camera_json["lookAt"][0].get<float>(),
            camera_json["lookAt"][1].get<float>(),
            camera_json["lookAt"][2].get<float>());
    }

    if (camera_json.contains("upVector"))
    {
        config.camera_up = Vec3(
            camera_json["upVector"][0].get<float>(),
            camera_json["upVector"][1].get<float>(),
            camera_json["upVector"][2].get<float>());
    }

    if (camera_json.contains("fov"))
    {
        config.fov = camera_json["fov"].get<float>();
    }

    if (camera_json.contains("width") && camera_json.contains("height"))
    {
        config.image_width = camera_json["width"].get<int>();
        config.image_height = camera_json["height"].get<int>();
    }

    scene.camera = std::make_shared<Camera>(
        config.camera_position,
        config.camera_lookat,
        config.camera_up,
        config.fov,
        static_cast<float>(config.image_width) / config.image_height,
        config.aperture,
        config.focus_distance,
        config.use_dof);

    scene.image = std::make_unique<Image>(config);
}

void SceneLoader::parse_scene(Scene &scene, SceneConfig &config, const nlohmann::json &scene_json)
{
    // Parse background color
    if (scene_json.contains("backgroundcolor") && !scene_json.contains("use_gradient"))
    {
        config.background_bottom = Vec3(
            scene_json["backgroundcolor"][0].get<float>(),
            scene_json["backgroundcolor"][1].get<float>(),
            scene_json["backgroundcolor"][2].get<float>());
        config.use_gradient = false;
    }
    else if (scene_json.contains("backgroundcolor") && scene_json.contains("use_gradient"))
    {
        config.background_bottom = Vec3(
            scene_json["backgroundcolortop"][0].get<float>(),
            scene_json["backgroundcolortop"][1].get<float>(),
            scene_json["backgroundcolortop"][2].get<float>());
        config.background_top = Vec3(
            scene_json["backgroundcolorbottom"][3].get<float>(),
            scene_json["backgroundcolorbottom"][4].get<float>(),
            scene_json["backgroundcolorbottom"][5].get<float>());
        config.use_gradient = true;
    }

    // Parse lights
    if (scene_json.contains("lightsources"))
    {
        parse_lights(scene, scene_json["lightsources"]);
    }

    // Parse shapes
    if (scene_json.contains("shapes"))
    {
        parse_shapes(scene, config, scene_json["shapes"]);
    }
}

void SceneLoader::parse_lights(Scene &scene, const nlohmann::json &lights_json)
{
    for (const auto &light_json : lights_json)
    {
        if (light_json.contains("type") && light_json["type"] == "pointlight")
        {
            Vec3 position(
                light_json["position"][0].get<float>(),
                light_json["position"][1].get<float>(),
                light_json["position"][2].get<float>());
            Vec3 intensity(
                light_json["intensity"][0].get<float>(),
                light_json["intensity"][1].get<float>(),
                light_json["intensity"][2].get<float>());

            scene.lights.push_back(std::make_shared<Light>(position, intensity));
        }
    }
}

void SceneLoader::parse_shapes(Scene &scene, SceneConfig &config, const nlohmann::json &shapes_json)
{
    for (const auto &shape_json : shapes_json)
    {
        std::shared_ptr<Material> material;

        // Check if the "material" key exists
        if (shape_json.contains("material"))
        {
            material = parse_material(config, shape_json["material"]);
        }
        else
        {
            material = std::make_shared<BinaryMaterial>(Vec3(1.0f, 0.0f, 0.0f));
            std::cerr
                << "Warning: Shape of type '" << shape_json["type"].get<std::string>()
                << "' is missing 'material' field. Using default red material." << std::endl;
        }

        // Continue parsing the shape
        if (!shape_json.contains("type"))
        {
            std::cerr << "Error: Shape is missing 'type' field." << std::endl;
            continue;
        }

        std::string type = shape_json["type"].get<std::string>();

        if (type == "sphere")
        {
            if (!shape_json.contains("center") || !shape_json.contains("radius"))
            {
                std::cerr << "Error: Sphere is missing 'center' or 'radius' field." << std::endl;
                continue;
            }

            Vec3 center = parse_vec3(shape_json["center"]);
            float radius = shape_json["radius"].get<float>();

            scene.objects.push_back(std::make_shared<Sphere>(center, radius, material));
        }
        else if (type == "cylinder")
        {
            if (!shape_json.contains("center") || !shape_json.contains("axis") ||
                !shape_json.contains("radius") || !shape_json.contains("height"))
            {
                std::cerr << "Error: Cylinder is missing required fields." << std::endl;
                continue;
            }

            Vec3 center = parse_vec3(shape_json["center"]);
            Vec3 axis = parse_vec3(shape_json["axis"]);
            float radius = shape_json["radius"].get<float>();
            float height = shape_json["height"].get<float>();

            scene.objects.push_back(std::make_shared<Cylinder>(center, axis, radius, height, material));
        }
        else if (type == "triangle")
        {
            if (!shape_json.contains("v0") || !shape_json.contains("v1") || !shape_json.contains("v2"))
            {
                std::cerr << "Error: Triangle is missing one of 'v0', 'v1', or 'v2' fields." << std::endl;
                continue;
            }

            Vec3 v0 = parse_vec3(shape_json["v0"]);
            Vec3 v1 = parse_vec3(shape_json["v1"]);
            Vec3 v2 = parse_vec3(shape_json["v2"]);

            scene.objects.push_back(std::make_shared<Triangle>(v0, v1, v2, material));
        }
        else if (type == "rectangle")
        {
            if (shape_json.contains("v0") && shape_json.contains("v1") &&
                shape_json.contains("v2") && shape_json.contains("v3"))
            {
                Vec3 v0 = parse_vec3(shape_json["v0"]);
                Vec3 v1 = parse_vec3(shape_json["v1"]);
                Vec3 v2 = parse_vec3(shape_json["v2"]);
                Vec3 v3 = parse_vec3(shape_json["v3"]);

                scene.objects.push_back(std::make_shared<Rectangle>(v0, v1, v2, v3, material));
            }
            else if (shape_json.contains("corner1") && shape_json.contains("corner2"))
            {
                Vec3 corner1 = parse_vec3(shape_json["corner1"]);
                Vec3 corner2 = parse_vec3(shape_json["corner2"]);

                scene.objects.push_back(std::make_shared<Rectangle>(corner1, corner2, material));
            }
            else
            {
                std::cerr << "Error: Rectangle is missing required fields." << std::endl;
                continue;
            }
        }
        else if (type == "box")
        {
            if (!shape_json.contains("min") || !shape_json.contains("max"))
            {
                std::cerr << "Error: Box is missing one of 'min' or 'max' fields." << std::endl;
                continue;
            }

            Vec3 min = parse_vec3(shape_json["min"]);
            Vec3 max = parse_vec3(shape_json["max"]);

            // Default rotation is 0 if not specified
            Vec3 rotation(0, 0, 0);
            if (shape_json.contains("rotation"))
            {
                rotation = parse_vec3(shape_json["rotation"]);
            }

            scene.objects.push_back(std::make_shared<Box>(min, max, rotation, material));
        }
        else
        {
            std::cerr << "Error: Unknown shape type '" << type << "'." << std::endl;
            continue;
        }
    }
}

std::shared_ptr<Material> SceneLoader::parse_material(SceneConfig &config, const nlohmann::json &material_json)
{
    // Check for binary render mode
    if (config.render_mode == RenderMode::BINARY)
    {
        return std::make_shared<BinaryMaterial>(Vec3(1.0f, 0.0f, 0.0f));
    }

    // Check for path-traced render mode
    if (config.render_mode == RenderMode::PATH)
    {
        std::string material_type = material_json.value("type", "");

        if (material_type == "dielectric")
        {
            float ior = material_json.value("ior", 1.5f); // Default IOR
            if (material_json.contains("ior"))
            {
                ior = material_json["ior"].get<float>();
            }
            return std::make_shared<Dielectric>(ior);
        }
        else if (material_type == "diffuse")
        {
            Vec3 albedo(1.0f); // Default albedo
            if (material_json.contains("albedo"))
            {
                albedo = Vec3(
                    material_json["albedo"][0].get<float>(),
                    material_json["albedo"][1].get<float>(),
                    material_json["albedo"][2].get<float>());
            }
            return std::make_shared<Diffuse>(albedo);
        }
        else if (material_type == "emissive")
        {
            Vec3 color(1.0f);                                         // Default colour
            float intensity = material_json.value("intensity", 1.0f); // Default intensity
            if (material_json.contains("color"))
            {
                color = Vec3(
                    material_json["color"][0].get<float>(),
                    material_json["color"][1].get<float>(),
                    material_json["color"][2].get<float>());
            }
            if (material_json.contains("color"))
            {
                intensity = material_json["intensity"][0].get<float>();
            }
            return std::make_shared<Emissive>(color, intensity);
        }
        else if (material_type == "metal")
        {
            Vec3 albedo(1.0f);                                        // Default albedo
            float roughness = material_json.value("roughness", 0.5f); // Default roughness
            if (material_json.contains("albedo"))
            {
                albedo = Vec3(
                    material_json["albedo"][0].get<float>(),
                    material_json["albedo"][1].get<float>(),
                    material_json["albedo"][2].get<float>());
            }
            return std::make_shared<Metal>(albedo, roughness);
        }
        else
        {
            throw std::runtime_error("Unsupported material type in path-traced render mode: " + material_type);
        }
    }

    // Default Blinn-Phong materials for non-path-traced modes
    Vec3 diffuse_color(0.5f);
    Vec3 specular_color(1.0f);
    float shininess = 32.0f;
    float kd = material_json.value("kd", 0.9f);
    float ks = material_json.value("ks", 0.1f);
    float ka = material_json.value("ka", 0.1f);
    bool is_reflective = material_json.value("isreflective", false);
    float reflectivity = material_json.value("reflectivity", 0.0f);
    bool is_refractive = material_json.value("isrefractive", false);
    float ior = material_json.value("refractiveindex", 1.0f);

    if (material_json.contains("diffusecolor"))
    {
        diffuse_color = Vec3(
            material_json["diffusecolor"][0].get<float>(),
            material_json["diffusecolor"][1].get<float>(),
            material_json["diffusecolor"][2].get<float>());
    }

    if (material_json.contains("specularcolor"))
    {
        specular_color = Vec3(
            material_json["specularcolor"][0].get<float>(),
            material_json["specularcolor"][1].get<float>(),
            material_json["specularcolor"][2].get<float>());
    }

    if (material_json.contains("specularexponent"))
    {
        shininess = material_json["specularexponent"].get<float>();
    }

    if (material_json.contains("texture"))
    {
        std::string texture_path = material_json["texture"].get<std::string>();
        auto diffuse_texture = std::make_shared<ImageTexture>(texture_path);

        return std::make_shared<BlinnPhongMaterial>(
            diffuse_texture,
            specular_color,
            shininess,
            kd,
            ks,
            ka,
            is_reflective,
            reflectivity,
            is_refractive,
            ior);
    }

    return std::make_shared<BlinnPhongMaterial>(
        diffuse_color,
        specular_color,
        shininess,
        kd,
        ks,
        ka,
        is_reflective,
        reflectivity,
        is_refractive,
        ior);
}

Vec3 SceneLoader::parse_vec3(const nlohmann::json &json_array)
{
    if (json_array.is_array() && json_array.size() == 3)
    {
        return Vec3(
            json_array[0].get<float>(),
            json_array[1].get<float>(),
            json_array[2].get<float>());
    }
    else
    {
        std::cerr << "Error: Expected an array of 3 elements for Vec3." << std::endl;
        return Vec3(0.0f);
    }
}
