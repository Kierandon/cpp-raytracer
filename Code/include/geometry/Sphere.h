#ifndef SPHERE_H
#define SPHERE_H

#include "geometry/Hittable.h"

/**
 * @class Sphere
 * @brief Represents a sphere in 3D space, used for ray tracing.
 *        The sphere is defined by a center point and a radius.
 *        The class supports ray intersection testing and bounding box calculation.
 */
class Sphere : public Hittable
{
public:
    /**
     * @brief Default constructor for a Sphere object.
     */
    Sphere() {}

    /**
     * @brief Constructs a Sphere with a specified center, radius, and material.
     * @param cen The center point of the sphere.
     * @param r The radius of the sphere.
     * @param mat The material associated with the sphere.
     */
    Sphere(const Vec3 &cen, float r, std::shared_ptr<Material> mat)
        : centre(cen), radius(r), material_ptr(mat) {}

    /**
     * @brief Checks if a ray intersects with the sphere.
     * @param ray The ray to test for intersection.
     * @param t_min The minimum distance for intersection.
     * @param t_max The maximum distance for intersection.
     * @param rec A reference to a HitRecord that will store information about the intersection.
     * @return True if the ray intersects the sphere, false otherwise.
     */
    virtual bool hit(const Ray &ray, float t_min, float t_max, HitRecord &rec) const override;

    /**
     * @brief Computes the bounding box of the sphere.
     * @param output_box The AABB to store the bounding box.
     * @return True if the bounding box is successfully computed, false otherwise.
     */
    virtual bool bounding_box(AABB &output_box) const override;

    Vec3 centre;                            ///< The center point of the sphere.
    float radius;                           ///< The radius of the sphere.
    std::shared_ptr<Material> material_ptr; ///< The material associated with the sphere.
};

#endif // SPHERE_H
