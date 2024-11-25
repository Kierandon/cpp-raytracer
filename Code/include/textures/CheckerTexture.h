#ifndef CHECKER_TEXTURE_H
#define CHECKER_TEXTURE_H

#include "textures/Texture.h"
#include <memory>
#include <cmath>

/**
 * @class CheckerTexture
 * @brief Represents a checkerboard texture, where alternating regions are mapped with two different textures.
 *        The checkerboard pattern is generated based on the 3D position, using a sine function to determine
 *        the "even" or "odd" regions of the checkerboard. This class provides a way to create procedural
 *        checker textures by blending two different textures in a repeating pattern.
 */
class CheckerTexture : public Texture
{
public:
    /**
     * @brief Constructs a CheckerTexture with two textures (one for the even regions and one for the odd regions).
     * @param t0 The texture to be applied to the even regions of the checkerboard.
     * @param t1 The texture to be applied to the odd regions of the checkerboard.
     * @param scale The scale of the checkerboard pattern, controlling how frequently the texture repeats (default is 10.0).
     */
    CheckerTexture(std::shared_ptr<Texture> t0, std::shared_ptr<Texture> t1, float scale = 10.0f)
        : even(t0), odd(t1), scale(scale) {}

    /**
     * @brief Retrieves the color of the texture at given texture coordinates (u, v) based on the 3D point `p`.
     *        The checkerboard pattern is determined by the 3D position of the point `p`. It uses sine functions
     *        on the `x`, `y`, and `z` coordinates to create a periodic pattern. Depending on the result, either
     *        the "even" or "odd" texture is applied.
     * @param u The u-coordinate of the texture (unused for this texture type).
     * @param v The v-coordinate of the texture (unused for this texture type).
     * @param p The 3D point in space used to calculate the checkerboard pattern.
     * @return The color of the texture at the specified coordinates.
     */
    virtual Vec3 value(float u, float v, const Vec3 &p) const override
    {
        // Calculate the sine value for the checkerboard pattern based on the 3D coordinates
        float sines = sin(scale * p.x) * sin(scale * p.y) * sin(scale * p.z);
        // Return either the "even" or "odd" texture based on the calculated sine value
        if (sines < 0)
            return odd->value(u, v, p); // Apply the odd texture
        else
            return even->value(u, v, p); // Apply the even texture
    }

private:
    std::shared_ptr<Texture> even; ///< The texture applied to the even regions of the checkerboard.
    std::shared_ptr<Texture> odd;  ///< The texture applied to the odd regions of the checkerboard.
    float scale;                   ///< The scale of the checkerboard pattern, controlling how frequently the textures alternate.
};

#endif // CHECKER_TEXTURE_H
