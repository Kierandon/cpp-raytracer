#include "core/Image.h"
#include "postprocess/BilateralDenoiser.h"
#include "scene/SceneLoader.h"

#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>

Image::Image(const SceneConfig &config) : config(config)
{
    pixels.resize(config.image_width * config.image_height, Vec3(0, 0, 0));

    if (config.use_tone_mapping)
    {
        tone_mapper = std::make_shared<ReinhardToneMapper>();
    }
}

void Image::set_pixel(int x, int y, const Vec3 &color)
{
    if (!is_valid_coords(x, y))
    {
        std::cerr << "Error: Pixel coordinates (" << x << "," << y << ") out of bounds\n";
        return;
    }
    pixels[y * config.image_width + x] = color;
}

Vec3 Image::get_pixel(int x, int y) const
{
    if (!is_valid_coords(x, y))
    {
        std::cerr << "Error: Pixel coordinates (" << x << "," << y << ") out of bounds\n";
        return Vec3(0, 0, 0);
    }
    return pixels[y * config.image_width + x];
}

void Image::clear(const Vec3 &color)
{
    std::fill(pixels.begin(), pixels.end(), color);
}

void Image::fill(const Vec3 &color)
{
    clear(color);
}

bool Image::save(const std::string &filename, ImageFormat format)
{
    switch (format)
    {
    case ImageFormat::PPM:
        return save_ppm(filename);
    default:
        std::cerr << "Error: Unsupported image format\n";
        return false;
    }
}

Vec3 Image::process_pixel(const Vec3 &color)
{
    // Apply tone mapping if available
    Vec3 mapped_color = tone_mapper ? tone_mapper->map(color) : color;

    // Apply gamma correction
    mapped_color.x = std::pow(mapped_color.x, 1.0f / gamma);
    mapped_color.y = std::pow(mapped_color.y, 1.0f / gamma);
    mapped_color.z = std::pow(mapped_color.z, 1.0f / gamma);

    // Clamp values
    return Vec3(
        mapped_color.x,
        mapped_color.y,
        mapped_color.z);
}

bool Image::save_ppm(const std::string &filename)
{
    if (config.use_denoiser)
    {
        std::cout << "Denoising image...\n";
        BilateralDenoiser denoiser(config, pixels);
        denoiser.denoise();
    }

    if (config.use_gaussian_blur) {
        std::cout << "Applying Gaussian Blur...\n";
        apply_gaussian_blur(config.blur_sigma, config.blur_kernel_size);
    }

    std::ofstream file(filename);
    if (!file)
    {
        std::cerr << "Error: Could not open file " << filename << " for writing\n";
        return false;
    }

    file << "P3\n"
         << config.image_width << " " << config.image_height << "\n255\n";

    for (int y = config.image_height - 1; y >= 0; --y)
    {
        for (int x = 0; x < config.image_width; ++x)
        {
            Vec3 processed = process_pixel(pixels[y * config.image_width + x]);
            file << static_cast<int>(255.999 * processed.x) << " "
                 << static_cast<int>(255.999 * processed.y) << " "
                 << static_cast<int>(255.999 * processed.z) << "\n";
        }
    }

    file.close();
    std::cout << "Image saved to " << filename << std::endl;
    return true;
}

bool Image::is_valid_coords(int x, int y) const
{
    return x >= 0 && x < config.image_width && y >= 0 && y < config.image_height;
}

void Image::apply_gaussian_blur(float sigma, int kernel_size)  {
    if (kernel_size % 2 == 0) kernel_size++;
    std::vector<float> kernel = create_gaussian_kernel(sigma, kernel_size);
    std::vector<Vec3> temp_buffer(config.image_width * config.image_height);
    
    std::cout << "Starting horizontal blur...\n";
    horizontal_blur(temp_buffer, kernel);
    std::cout << "Horizontal blur completed.\n";

    std::cout << "Starting vertical blur...\n";
    vertical_blur(temp_buffer, kernel);
    std::cout << "Vertical blur completed.\n";

    std::cout << "Gaussian blur applied successfully.\n";
}

std::vector<float> Image::create_gaussian_kernel(float sigma, int size) {
    std::vector<float> kernel(size);
    float sum = 0.0f;
    int half = size / 2;
    
    // Calculate kernel values
    for (int i = 0; i < size; i++) {
        float x = float(i - half);
        kernel[i] = std::exp(-(x * x) / (2.0f * sigma * sigma));
        sum += kernel[i];
    }
    
    // Normalize kernel
    for (int i = 0; i < size; i++) {
        kernel[i] /= sum;
    }
    
    return kernel;
}

void Image::horizontal_blur(std::vector<Vec3>& temp_buffer, const std::vector<float>& kernel) {
    int half = kernel.size() / 2;
    
    for (int y = 0; y < config.image_height; y++) {
        for (int x = 0; x < config.image_width; x++) {
            Vec3 sum(0, 0, 0);
            
            for (int k = -half; k <= half; k++) {
                int sx = std::clamp(x + k, 0, config.image_width - 1);
                sum += get_pixel(sx, y) * kernel[k + half];
            }
            
            temp_buffer[y * config.image_width + x] = sum;
        }
        
        // Print progress
        if (y % (config.image_height / 10) == 0) {
            int progress = (y * 100) / config.image_height;
            std::cout << "Horizontal blur progress: " << progress << "%\n";
        }
    }
    
    // Copy back to pixels
    for (int y = 0; y < config.image_height; y++) {
        for (int x = 0; x < config.image_width; x++) {
            set_pixel(x, y, temp_buffer[y * config.image_width + x]);
        }
    }
}

void Image::vertical_blur(std::vector<Vec3>& temp_buffer, const std::vector<float>& kernel) {
    int half = kernel.size() / 2;
    
    for (int y = 0; y < config.image_height; y++) {
        for (int x = 0; x < config.image_width; x++) {
            Vec3 sum(0, 0, 0);
            
            for (int k = -half; k <= half; k++) {
                int sy = std::clamp(y + k, 0, config.image_height - 1);
                sum += get_pixel(x, sy) * kernel[k + half];
            }
            
            temp_buffer[y * config.image_width + x] = sum;
        }
        
        // Print progress
        if (y % (config.image_height / 10) == 0) {
            int progress = (y * 100) / config.image_height;
            std::cout << "Vertical blur progress: " << progress << "%\n";
        }
    }
    
    // Copy back to pixels
    for (int y = 0; y < config.image_height; y++) {
        for (int x = 0; x < config.image_width; x++) {
            set_pixel(x, y, temp_buffer[y * config.image_width + x]);
        }
    }
}
