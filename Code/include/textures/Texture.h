#ifndef TEXTURE_H
#define TEXTURE_H

#include "core/Vec3.h"

/**
 * @class Texture
 * @brief Abstract base class for textures in a 3D rendering system.
 *        Textures are used to map colors or patterns onto surfaces, usually through UV coordinates.
 *        This class defines the interface for different types of textures (e.g., image textures, procedural textures).
 */
class Texture
{
public:
    /**
     * @brief Virtual destructor for the Texture class.
     */
    virtual ~Texture() = default;

    /**
     * @brief Pure virtual function to retrieve the color value at given texture coordinates.
     *        This function must be implemented by derived classes to provide specific texture behavior.
     * @param u The u-coordinate of the texture (typically between 0 and 1).
     * @param v The v-coordinate of the texture (typically between 0 and 1).
     * @param p A point in world space that may be used to modify the texture value (e.g., for procedural textures).
     * @return The color value of the texture at the specified coordinates.
     */
    virtual Vec3 value(float u, float v, const Vec3 &p) const = 0;
};

#endif // TEXTURE_H
