#ifndef LIGHT_H
#define LIGHT_H

#include "core/Vec3.h"

/**
 * @class Light
 * @brief Represents a light source in 3D space, defined by its position and intensity.
 *        The class includes a method for sampling the light direction, color, and distance
 *        from a given point in the scene.
 */
class Light
{
public:
    /**
     * @brief Constructs a Light object with a specified position and intensity.
     * @param position The position of the light source in 3D space.
     * @param intensity The intensity (or color) of the light at the position.
     */
    Light(const Vec3 &position, const Vec3 &intensity)
        : position(position), intensity(intensity) {}

    /**
     * @brief Samples the light at a specific hit point.
     *        This calculates the direction of the light, its color, and the distance from the light to the point.
     * @param hit_point The point in space where the light is being sampled.
     * @param light_direction The direction from the hit point to the light source (output).
     * @param light_color The color (intensity) of the light at the hit point (output).
     * @param distance The distance from the hit point to the light source (output).
     */
    void sample(const Vec3 &hit_point,
                Vec3 &light_direction,
                Vec3 &light_color,
                float &distance) const
    {
        light_direction = (position - hit_point).normalized(); ///< Calculate the normalized direction.
        light_color = intensity;                               ///< The intensity is the light color.
        distance = (position - hit_point).length();            ///< Calculate the distance between the hit point and the light.
    }

    Vec3 position;  ///< The position of the light source in 3D space.
    Vec3 intensity; ///< The intensity or color of the light at the source.
};

#endif // LIGHT_H
