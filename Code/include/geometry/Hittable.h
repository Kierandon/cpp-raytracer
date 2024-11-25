#ifndef HITTABLE_H
#define HITTABLE_H

#include "core/Ray.h"
#include "lighting/HitRecord.h"
#include "geometry/AABB.h"

/**
 * @class Hittable
 * @brief Abstract base class representing an object that can be hit by a ray.
 *        This class defines the interface for geometric objects that support ray intersection testing
 *        and bounding box computation.
 */
class Hittable
{
public:
    /**
     * @brief Virtual destructor for the Hittable class.
     */
    virtual ~Hittable() = default;

    /**
     * @brief Pure virtual function to check if a ray intersects with the object.
     * @param ray The ray to test for intersection.
     * @param t_min The minimum distance for intersection.
     * @param t_max The maximum distance for intersection.
     * @param rec A reference to a HitRecord that will store information about the intersection.
     * @return True if the ray intersects the object, false otherwise.
     */
    virtual bool hit(const Ray &ray, float t_min, float t_max, HitRecord &rec) const = 0;

    /**
     * @brief Pure virtual function to compute the bounding box of the object.
     * @param output_box The AABB to store the bounding box.
     * @return True if the bounding box is successfully computed, false otherwise.
     */
    virtual bool bounding_box(AABB &output_box) const = 0;
};

#endif // HITTABLE_H
