#ifndef AABB_H
#define AABB_H

#include "core/Vec3.h"
#include "core/Ray.h"

/**
 * @class AABB
 * @brief Axis-Aligned Bounding Box for 3D geometry.
 */
class AABB
{
public:
    /**
     * @brief Default constructor.
     * Creates an uninitialised bounding box.
     */
    AABB();

    /**
     * @brief Parameterised constructor.
     * @param min Minimum corner of the bounding box.
     * @param max Maximum corner of the bounding box.
     */
    AABB(const Vec3 &min, const Vec3 &max);

    /**
     * @brief Tests if a ray intersects with the bounding box.
     *
     * @param ray The ray to test against the bounding box.
     * @param t_min Minimum t-value of the ray interval to consider.
     * @param t_max Maximum t-value of the ray interval to consider.
     * @return True if the ray intersects the bounding box within the specified interval, false otherwise.
     */
    bool hit(const Ray &ray, float t_min, float t_max) const;

    /**
     * @brief Creates a bounding box that surrounds two given bounding boxes.
     *
     * @param box0 The first bounding box.
     * @param box1 The second bounding box.
     * @return A new bounding box that completely encloses both input boxes.
     */
    static AABB surrounding_box(const AABB &box0, const AABB &box1);

    /// Minimum corner of the bounding box.
    Vec3 minimum;

    /// Maximum corner of the bounding box.
    Vec3 maximum;
};

#endif
