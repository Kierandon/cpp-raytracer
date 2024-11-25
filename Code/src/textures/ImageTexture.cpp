#include "textures/ImageTexture.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>

ImageTexture::ImageTexture(const std::string &filename)
{
    if (!load_ppm(filename))
    {
        std::cerr << "Failed to load texture: " << filename << std::endl;
    }
}

bool ImageTexture::load_ppm(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file)
        return false;

    // Read PPM header
    std::string magic_number;
    file >> magic_number;
    if (magic_number != "P3")
        return false;

    skip_comments(file);
    file >> width >> height;

    int max_value;
    file >> max_value;

    // Read pixel data
    data.reserve(width * height * 3);
    int r, g, b;
    while (file >> r >> g >> b)
    {
        data.push_back(static_cast<unsigned char>(r));
        data.push_back(static_cast<unsigned char>(g));
        data.push_back(static_cast<unsigned char>(b));
    }

    return true;
}

void ImageTexture::skip_comments(std::ifstream &file)
{
    char c;
    while (file.get(c))
    {
        if (c == '#')
        {
            std::string comment;
            std::getline(file, comment);
        }
        else if (!std::isspace(c))
        {
            file.unget();
            break;
        }
    }
}

Vec3 ImageTexture::value(float u, float v, [[maybe_unused]] const Vec3 &p) const
{
    u = std::clamp(u, 0.0f, 1.0f);
    v = std::clamp(v, 0.0f, 1.0f);

    int x = static_cast<int>(u * width);
    int y = static_cast<int>((1 - v) * height - 0.001f);

    x = std::clamp(x, 0, width - 1);
    y = std::clamp(y, 0, height - 1);

    return get_pixel(x, y);
}

Vec3 ImageTexture::get_pixel(int x, int y) const
{
    const float color_scale = 1.0f / 255.0f;
    int index = (y * width + x) * bytes_per_pixel;

    return Vec3(
        color_scale * data[index],
        color_scale * data[index + 1],
        color_scale * data[index + 2]);
}
