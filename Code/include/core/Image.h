#ifndef IMAGE_H
#define IMAGE_H

#include "core/Vec3.h"
#include "postprocess/ToneMapper.h"
#include "scene/SceneConfig.h"
#include "postprocess/ReinhardToneMapper.h"

#include <vector>
#include <memory>
#include <string>

enum class ImageFormat
{
    PPM,
};

/**
 * @class Image
 * @brief A class representing an image that can be manipulated and saved in various formats.
 *
 * The Image class provides functionalities to set and get pixel values, clear and fill the image,
 * apply tone mapping and gamma correction, and apply Gaussian blur. It also supports saving the
 * image in different formats.
 */
class Image
{
public:
    explicit Image(const SceneConfig &config);

    void set_pixel(int x, int y, const Vec3 &color);
    Vec3 get_pixel(int x, int y) const;
    void clear(const Vec3 &color = Vec3(0, 0, 0));
    void fill(const Vec3 &color);

    void set_tone_mapper(std::shared_ptr<ToneMapper> mapper) { tone_mapper = mapper; }
    void set_gamma(float value) { gamma = value; }
    void apply_gaussian_blur(float sigma, int kernel_size);

    bool save(const std::string &filename, ImageFormat format = ImageFormat::PPM);
    bool save_ppm(const std::string &filename);

private:
    Vec3 process_pixel(const Vec3 &color);
    bool is_valid_coords(int x, int y) const;
    std::vector<Vec3> pixels;
    std::shared_ptr<ToneMapper> tone_mapper;
    const SceneConfig &config;
    float gamma{1.2f};
    std::vector<float> create_gaussian_kernel(float sigma, int size);
    void horizontal_blur(std::vector<Vec3>& temp_buffer, const std::vector<float>& kernel);
    void vertical_blur(std::vector<Vec3>& temp_buffer, const std::vector<float>& kernel);
};

#endif
