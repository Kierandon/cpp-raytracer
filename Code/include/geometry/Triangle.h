#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "geometry/Hittable.h"

/**
 * @class Triangle
 * @brief Represents a triangle in 3D space, used for ray tracing.
 *        The triangle is defined by three vertices and a normal vector.
 *        The class supports ray intersection testing and bounding box calculation.
 */
class Triangle : public Hittable
{
public:
    /**
     * @brief Default constructor for a Triangle object.
     */
    Triangle() {}

    /**
     * @brief Constructs a Triangle with three vertices and a material.
     * @param v0 The first vertex of the triangle.
     * @param v1 The second vertex of the triangle.
     * @param v2 The third vertex of the triangle.
     * @param mat The material associated with the triangle.
     */
    Triangle(const Vec3 &v0, const Vec3 &v1, const Vec3 &v2, std::shared_ptr<Material> mat)
        : vertex0(v0), vertex1(v1), vertex2(v2), material_ptr(mat)
    {
        // Calculate the normal vector of the triangle by taking the cross product of two edges
        normal = (v1 - v0).cross(v2 - v0).normalized();
    }

    /**
     * @brief Checks if a ray intersects with the triangle.
     * @param ray The ray to test for intersection.
     * @param t_min The minimum distance for intersection.
     * @param t_max The maximum distance for intersection.
     * @param rec A reference to a HitRecord that will store information about the intersection.
     * @return True if the ray intersects the triangle, false otherwise.
     */
    virtual bool hit(const Ray &ray, float t_min, float t_max, HitRecord &rec) const override;

    /**
     * @brief Computes the bounding box of the triangle.
     * @param output_box The AABB to store the bounding box.
     * @return True if the bounding box is successfully computed, false otherwise.
     */
    virtual bool bounding_box(AABB &output_box) const override;

private:
    Vec3 vertex0, vertex1, vertex2;         ///< The three vertices of the triangle.
    Vec3 normal;                            ///< The normal vector of the triangle, calculated from the vertices.
    std::shared_ptr<Material> material_ptr; ///< The material associated with the triangle.
};

#endif // TRIANGLE_H
