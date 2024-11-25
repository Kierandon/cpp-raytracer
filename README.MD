# Ray/Pathtracer

A ray/pathtracer developed for Computer Graphics (CGR24) at UoE that supports various rendering features and scene configurations.

## Features

### Core Features

- Image writing and output
- Virtual pin-hole camera
- Primitive intersection tests (Sphere, Triangle, Cylinder, Rectangle, Box)
- Blinn-Phong shading model
- Shadow ray casting
- Reinhard tone mapping
- Perfect reflection
- Perfect refraction with Fresnel effects

### Advanced Features

- Texture mapping support
- BVH acceleration structure
- Path tracing with:
  - Multiple importance sampling
  - Pixel sampling strategies
  - Depth of field effects
  - BRDF sampling
  - Light sampling

## Building

To build the project:

```bash
cd code
make
```

## Running Scenes

Execute the raytracer with a scene configuration file:

```bash
./raytracer <scene_file.json>
```

### Example

```bash
./raytracer scenes/cornell_box.json
```

## Scene Configuration

Scenes are defined using JSON files. Below is the structure of a sample configuration:

```json
{
    "nbounces": 50,
    "nsamples": 1000,
    "rendermode": "path",
    "use_tone_mapping": true,
    "use_stratified_sampling": true,
    "use_bvh": true,
    "camera": {
        "type": "pinhole",
        "width": 1000,
        "height": 1000,
        "position": [278, 278, -800],
        "lookAt": [278, 278, 0],
        "upVector": [0, 1, 0],
        "fov": 40.0,
        "exposure": 0.1
    },
    "scene": {
        "backgroundcolor": [0.0, 0.0, 0.0],
        "lightsources": [...],
        "shapes": [...]
    }
}
```

All available scene configuration options can be found in SceneConfig.h.

### Available Materials

Here are the supported material types and their configurations for different rendering modes:

#### Materials for Pathtracer

##### Diffuse

```json
{
  "type": "diffuse",
  "albedo": [0.73, 0.73, 0.73]
}
```

##### Metal

```json
{
  "type": "metal",
  "albedo": [0.9, 0.73, 0.73]
}
```

##### Glass (Dielectric)

```json
{
  "type": "dielectric",
  "ior": 1.5
}
```

##### Emissive

```json
{
  "type": "emissive",
  "colour": [1.0, 1.0, 0.8],
  "intensity": 12
}
```

##### Textured

```json
{
  "texture": "path/to/texture.ppm",
  "ks": 0.1,
  "kd": 0.9
}
```

---

#### Materials for Phong Render Mode

Phong materials are configured with properties specific to Blinn-Phong shading:

##### Example Material Configuration for Sphere

```json
{
  "type": "sphere",
  "center": [0, -25.0, 0],
  "radius": 25.1,
  "material": {
    "ks": 0.1,
    "kd": 0.9,
    "specularexponent": 10,
    "diffusecolor": [0.5, 1, 0.5],
    "specularcolor": [1.0, 1.0, 1.0],
    "isreflective": false,
    "reflectivity": 1.0,
    "isrefractive": false,
    "refractiveindex": 1.0
  }
}
```

##### Example Material Configuration for Cylinder

```json
{
  "type": "cylinder",
  "center": [-0.3, 0.19, 1],
  "axis": [0, 1, 0],
  "radius": 0.15,
  "height": 0.2,
  "material": {
    "ks": 0.1,
    "kd": 0.9,
    "specularexponent": 20,
    "diffusecolor": [0.5, 0.5, 0.8],
    "specularcolor": [1.0, 1.0, 1.0],
    "isreflective": false,
    "reflectivity": 1.0,
    "isrefractive": false,
    "refractiveindex": 1.0
  }
}
```

---

#### Binary Mode

In binary mode, materials are overridden with a solid colour for visualising shape intersections. No additional configuration is needed beyond defining the shape.

##### Example:

```json
{
  "type": "sphere",
  "center": [0, 0, 0],
  "radius": 10
}
```

## Example Scenes

- `scenes/cornell_box.json` - Classic Cornell Box with path tracing
- `scenes/path_dof.json` - Depth of field demonstration
- `scenes/scene_textures.json` - Texture mapping examples
- `scenes/scene_reflective.json` - Perfect reflection demo
- `scenes/binary_primitives.json` - Basic shape intersection tests
