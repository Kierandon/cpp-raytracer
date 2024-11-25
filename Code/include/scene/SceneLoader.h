#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H

#include "scene/Scene.h"
#include "scene/SceneConfig.h"
#include "geometry/BVHNode.h"
#include "geometry/HittableList.h"
#include <nlohmann/json.hpp>

/**
 * @class SceneLoader
 * @brief Responsible for loading and setting up scenes from different sources.
 *        This class provides functionality to load default scenes or custom scenes from JSON files.
 *        It also handles parsing scene components like the camera, lights, and shapes from the JSON structure.
 */
class SceneLoader
{
public:
    /**
     * @brief Loads the default scene setup, including all necessary components such as the camera, lights, and objects.
     * @param scene The scene to populate with default data.
     * @param config The scene configuration, which may contain specific rendering settings.
     */
    void load_default_scene(Scene &scene, SceneConfig &config);

    /**
     * @brief Loads a custom scene from a JSON file and sets up the scene components accordingly.
     * @param scene The scene to populate with the data from the JSON file.
     * @param config The scene configuration, which may contain specific rendering settings.
     * @param json_path The path to the JSON file containing the scene data.
     */
    void load_scene_from_json(Scene &scene, SceneConfig &config, const std::string &json_path);

private:
    /**
     * @brief Builds the scene by setting up all necessary components, such as the camera, lights, and objects.
     * @param scene The scene to build.
     * @param config The scene configuration.
     */
    void build_scene(Scene &scene, SceneConfig &config);

    /**
     * @brief Sets up the default scene with predefined settings.
     * @param scene The scene to populate with default data.
     * @param config The scene configuration to apply the default settings.
     */
    void setup_default_scene(Scene &scene, SceneConfig &config);

    /**
     * @brief Sets up a custom scene by parsing the data from a JSON file.
     * @param scene The scene to populate with custom data.
     * @param config The scene configuration.
     * @param json_path The path to the JSON file containing the scene data.
     */
    void setup_custom_scene(Scene &scene, SceneConfig &config, const std::string &json_path);

    /**
     * @brief Parses the JSON structure and extracts the scene data to populate the scene and configuration.
     * @param scene The scene to populate with data.
     * @param config The scene configuration.
     * @param json The parsed JSON structure containing scene data.
     */
    void parse_json(Scene &scene, SceneConfig &config, nlohmann::json &json);

    /**
     * @brief Parses the camera data from the JSON file and sets up the camera in the scene.
     * @param scene The scene to apply the camera data.
     * @param config The scene configuration.
     * @param camera_json The JSON data containing the camera configuration.
     */
    void parse_camera(Scene &scene, SceneConfig &config, const nlohmann::json &camera_json);

    /**
     * @brief Parses general scene data from the JSON structure and adds it to the scene.
     * @param scene The scene to populate with the parsed data.
     * @param config The scene configuration.
     * @param scene_json The JSON data containing the scene properties.
     */
    void parse_scene(Scene &scene, SceneConfig &config, const nlohmann::json &scene_json);

    /**
     * @brief Parses the lights data from the JSON structure and adds light sources to the scene.
     * @param scene The scene to populate with light sources.
     * @param lights_json The JSON data containing light configurations.
     */
    void parse_lights(Scene &scene, const nlohmann::json &lights_json);

    /**
     * @brief Parses the shapes (geometry) data from the JSON structure and adds the shapes to the scene.
     * @param scene The scene to populate with shapes.
     * @param config The scene configuration.
     * @param shapes_json The JSON data containing shape configurations.
     */
    void parse_shapes(Scene &scene, SceneConfig &config, const nlohmann::json &shapes_json);

    /**
     * @brief Parses material data from the JSON structure and returns a material object.
     * @param config The scene configuration.
     * @param material_json The JSON data containing material properties.
     * @return A shared pointer to the material created from the JSON data.
     */
    std::shared_ptr<Material> parse_material(SceneConfig &config, const nlohmann::json &material_json);

    /**
     * @brief Parses a Vec3 (3D vector) from a JSON array and returns it as a Vec3 object.
     * @param json_array The JSON array representing the 3D vector.
     * @return A Vec3 object corresponding to the parsed JSON data.
     */
    Vec3 parse_vec3(const nlohmann::json &json_array);
};

#endif // SCENE_LOADER_H
