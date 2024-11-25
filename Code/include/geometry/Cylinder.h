#ifndef CYLINDER_H
#define CYLINDER_H

#include "geometry/Hittable.h"

/**
 * @class Cylinder
 * @brief Represents a cylinder in 3D space, used for ray tracing.
 *        The cylinder is defined by a base center point, an axis of orientation, a radius, and a height.
 *        The class also supports ray intersection tests and bounding box calculation.
 */
class Cylinder : public Hittable
{
public:
    /**
     * @brief Default constructor for a Cylinder object.
     */
    Cylinder() {}

    /**
     * @brief Constructs a Cylinder with a specified base, axis, radius, height, and material.
     * @param base The center point of the base of the cylinder.
     * @param axis The axis of the cylinder, indicating the direction of its height.
     * @param r The radius of the cylinder.
     * @param h The height of the cylinder.
     * @param mat The material associated with the cylinder.
     */
    Cylinder(const Vec3 &base, const Vec3 &axis, float r, float h, std::shared_ptr<Material> mat)
        : center(base), axis(axis.normalized()), radius(r), height(h), material_ptr(mat) {}

    /**
     * @brief Checks if a ray intersects with the cylinder.
     * @param ray The ray to test for intersection.
     * @param t_min The minimum distance for intersection.
     * @param t_max The maximum distance for intersection.
     * @param rec A reference to a HitRecord that will store information about the intersection.
     * @return True if the ray intersects the cylinder, false otherwise.
     */
    virtual bool hit(const Ray &ray, float t_min, float t_max, HitRecord &rec) const override;

    /**
     * @brief Computes the bounding box of the cylinder.
     * @param output_box The AABB to store the bounding box.
     * @return True if the bounding box is successfully computed, false otherwise.
     */
    virtual bool bounding_box(AABB &output_box) const override;

private:
    Vec3 center;                            ///< The center point of the base of the cylinder.
    Vec3 axis;                              ///< The direction axis of the cylinder (normalized).
    float radius;                           ///< The radius of the cylinder.
    float height;                           ///< The height of the cylinder.
    std::shared_ptr<Material> material_ptr; ///< The material associated with the cylinder.
};

#endif // CYLINDER_H
