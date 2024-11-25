#ifndef PLANE_H
#define PLANE_H

#include "geometry/Hittable.h"

/**
 * @class Plane
 * @brief Represents a plane in 3D space, used for ray tracing.
 *        The plane is defined by a point on the plane and a normal vector.
 *        The class also supports ray intersection testing and bounding box calculation.
 */
class Plane : public Hittable
{
public:
    /**
     * @brief Default constructor for a Plane object.
     */
    Plane() {}

    /**
     * @brief Constructs a Plane with a specified point, normal vector, and material.
     * @param p The point on the plane.
     * @param n The normal vector of the plane.
     * @param mat The material associated with the plane.
     */
    Plane(const Vec3 &p, const Vec3 &n, std::shared_ptr<Material> mat)
        : point(p), normal(n.normalized()), material_ptr(mat) {}

    /**
     * @brief Checks if a ray intersects with the plane.
     * @param ray The ray to test for intersection.
     * @param t_min The minimum distance for intersection.
     * @param t_max The maximum distance for intersection.
     * @param rec A reference to a HitRecord that will store information about the intersection.
     * @return True if the ray intersects the plane, false otherwise.
     */
    virtual bool hit(const Ray &ray, float t_min, float t_max, HitRecord &rec) const override;

    /**
     * @brief Computes the bounding box of the plane.
     * @param output_box The AABB to store the bounding box.
     * @return True if the bounding box is successfully computed, false otherwise.
     */
    virtual bool bounding_box(AABB &output_box) const override;

private:
    Vec3 point;                             ///< A point on the plane.
    Vec3 normal;                            ///< The normal vector of the plane (normalized).
    std::shared_ptr<Material> material_ptr; ///< The material associated with the plane.
};

#endif // PLANE_H
