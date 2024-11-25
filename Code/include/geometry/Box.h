#ifndef BOX_H
#define BOX_H

#include "geometry/Hittable.h"
#include "geometry/Triangle.h"
#include <memory>
#include <vector>

/**
 * @class Box
 * @brief Represents an axis-aligned box, used for ray tracing.
 *        The box can be rotated and intersected with rays, as well as calculate its bounding box.
 */
class Box : public Hittable
{
public:
    /**
     * @brief Constructs a Box object with a specified minimum and maximum corner, rotation, and material.
     * @param min The minimum corner of the box.
     * @param max The maximum corner of the box.
     * @param rotation_deg The rotation of the box in degrees along each axis.
     * @param mat The material associated with the box.
     */
    Box(const Vec3 &min, const Vec3 &max, const Vec3 &rotation_deg, std::shared_ptr<Material> mat);

    /**
     * @brief Checks if a ray intersects with the box.
     * @param ray The ray to test for intersection.
     * @param t_min The minimum distance for intersection.
     * @param t_max The maximum distance for intersection.
     * @param rec A reference to a HitRecord that will store information about the intersection.
     * @return True if the ray intersects with the box, false otherwise.
     */
    virtual bool hit(const Ray &ray, float t_min, float t_max, HitRecord &rec) const override;

    /**
     * @brief Calculates the bounding box of the box.
     * @param output_box The AABB to store the bounding box.
     * @return True if the bounding box is successfully calculated, false otherwise.
     */
    virtual bool bounding_box(AABB &output_box) const override;

private:
    Vec3 min;                                         ///< The minimum corner of the box.
    Vec3 max;                                         ///< The maximum corner of the box.
    Vec3 rotation;                                    ///< The rotation of the box in degrees along each axis.
    std::vector<std::shared_ptr<Triangle>> triangles; ///< List of triangles that make up the box.
    std::shared_ptr<Material> material_ptr;           ///< The material associated with the box.

    /**
     * @brief Rotates a given point by the box's rotation.
     * @param point The point to rotate.
     * @return The rotated point.
     */
    Vec3 rotatePoint(const Vec3 &point) const;
};

#endif // BOX_H
