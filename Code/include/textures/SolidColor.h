#ifndef SOLID_COLOR_H
#define SOLID_COLOR_H

#include "textures/Texture.h"

/**
 * @class SolidColor
 * @brief Represents a solid color texture where every point on the texture is the same color.
 *        This texture type is useful for applying a uniform color to objects, such as simple colored materials.
 *        The texture does not vary based on the texture coordinates, providing a constant color value.
 */
class SolidColor : public Texture
{
public:
    /**
     * @brief Constructs a SolidColor texture with a specified color.
     * @param c The color of the texture, represented as a Vec3 (R, G, B).
     */
    SolidColor(const Vec3 &c) : color(c) {}

    /**
     * @brief Constructs a SolidColor texture with specified RGB values.
     * @param r The red component of the color.
     * @param g The green component of the color.
     * @param b The blue component of the color.
     */
    SolidColor(float r, float g, float b) : color(Vec3(r, g, b)) {}

    /**
     * @brief Returns the color of the texture. Since this is a solid color texture,
     *        the color is constant and does not depend on the texture coordinates or point `p`.
     * @param u The u-coordinate of the texture (unused for solid color).
     * @param v The v-coordinate of the texture (unused for solid color).
     * @param p A point in world space (unused for solid color).
     * @return The color of the texture (constant for all points).
     */
    virtual Vec3 value([[maybe_unused]] float u,
                       [[maybe_unused]] float v,
                       [[maybe_unused]] const Vec3 &p) const override
    {
        return color;
    }

private:
    Vec3 color; ///< The color of the texture, which is constant for all points.
};

#endif // SOLID_COLOR_H
