#ifndef RAY_H
#define RAY_H

#include "core/Vec3.h"

/**
 * @class Ray
 * @brief Represents a ray in 3D space with an origin and direction.
 *
 * The Ray class encapsulates a ray defined by an origin point and a direction vector.
 */

class Ray
{
public:
    /**
     * @brief Default constructor for the Ray class.
     *
     * Constructs a Ray object with default origin and direction.
     */
    Ray();

    /**
     * @brief Parameterized constructor for the Ray class.
     *
     * Constructs a Ray object with the specified origin and direction.
     *
     * @param origin The origin point of the ray.
     * @param direction The direction vector of the ray.
     */
    Ray(const Vec3 &origin, const Vec3 &direction);

    /**
     * @brief Gets the origin of the ray.
     *
     * @return The origin point of the ray.
     */
    Vec3 origin() const;

    /**
     * @brief Gets the direction of the ray.
     *
     * @return The direction vector of the ray.
     */
    Vec3 direction() const;

    /**
     * @brief Computes a point along the ray at a given parameter `t`.
     *
     * @param t The parameter along the ray.
     * @return The point along the ray at parameter `t`.
     */
    Vec3 at(float t) const;

private:
    Vec3 orig;
    Vec3 dir;
};

#endif // RAY_H
