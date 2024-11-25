#ifndef IMAGE_TEXTURE_H
#define IMAGE_TEXTURE_H

#include "textures/Texture.h"
#include <string>
#include <vector>
#include <fstream>

/**
 * @class ImageTexture
 * @brief Represents a texture loaded from an image file, typically in PPM format.
 *        This texture maps colors based on the pixel values of an image, allowing for detailed
 *        patterns and images to be applied to 3D objects.
 */
class ImageTexture : public Texture
{
public:
    /**
     * @brief Constructs an ImageTexture by loading an image from a file.
     *        The image should be in PPM format (Portable Pixmap).
     * @param filename The path to the image file to be loaded.
     */
    ImageTexture(const std::string &filename);

    /**
     * @brief Retrieves the color value of the texture at given texture coordinates (u, v).
     *        The function maps the UV coordinates to the corresponding pixel in the image,
     *        returning the color from the image data.
     * @param u The u-coordinate of the texture, typically between 0 and 1.
     * @param v The v-coordinate of the texture, typically between 0 and 1.
     * @param p A point in 3D space, typically unused for this texture type.
     * @return The color of the texture at the given coordinates, represented as a Vec3.
     */
    virtual Vec3 value(float u, float v, [[maybe_unused]] const Vec3 &p) const override;

private:
    std::vector<unsigned char> data; ///< The raw pixel data loaded from the image file.
    int width{0};                    ///< The width of the image in pixels.
    int height{0};                   ///< The height of the image in pixels.
    int bytes_per_pixel{3};          ///< The number of bytes per pixel (typically 3 for RGB images).

    /**
     * @brief Loads an image from a PPM file format.
     *        This function reads the image data from a PPM file and stores it in the texture's data array.
     * @param filename The path to the PPM image file.
     * @return True if the image was loaded successfully, false otherwise.
     */
    bool load_ppm(const std::string &filename);

    /**
     * @brief Skips over comments in a PPM file while reading the image.
     *        PPM files may contain comments that start with '#' which should be ignored during reading.
     * @param file The input file stream for the PPM file.
     */
    void skip_comments(std::ifstream &file);

    /**
     * @brief Retrieves the color of a specific pixel at the given coordinates.
     *        This function retrieves the color stored in the texture's data array for the pixel at (x, y).
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     * @return The color of the pixel as a Vec3 (RGB).
     */
    Vec3 get_pixel(int x, int y) const;
};

#endif
