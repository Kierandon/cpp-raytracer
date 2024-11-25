#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "core/Vec3.h"
#include "core/Ray.h"
#include "geometry/Hittable.h"
#include "geometry/Triangle.h"
#include <memory>
#include <vector>

/**
 * @class Rectangle
 * @brief Represents a rectangle in 3D space, defined by either four vertices or two diagonal corners.
 *        The rectangle is subdivided into two triangles for ray tracing purposes.
 *        It supports ray intersection tests and bounding box calculation.
 */
class Rectangle : public Hittable
{
public:
    /**
     * @brief Constructs a Rectangle from four vertices, which are used to create two triangles.
     * @param v0 The first vertex of the rectangle.
     * @param v1 The second vertex of the rectangle.
     * @param v2 The third vertex of the rectangle.
     * @param v3 The fourth vertex of the rectangle.
     * @param mat The material associated with the rectangle.
     */
    Rectangle(
        const Vec3 &v0,
        const Vec3 &v1,
        const Vec3 &v2,
        const Vec3 &v3,
        std::shared_ptr<Material> mat)
        : material_ptr(mat)
    {
        // Subdivide the rectangle into two triangles
        triangles.push_back(std::make_shared<Triangle>(v0, v1, v2, material_ptr));
        triangles.push_back(std::make_shared<Triangle>(v0, v2, v3, material_ptr));
    }

    /**
     * @brief Constructs a Rectangle from two diagonal corners (top-left and bottom-right).
     * @param topLeft The top-left corner of the rectangle.
     * @param bottomRight The bottom-right corner of the rectangle.
     * @param mat The material associated with the rectangle.
     */
    Rectangle(
        const Vec3 &topLeft,
        const Vec3 &bottomRight,
        std::shared_ptr<Material> mat)
        : material_ptr(mat)
    {
        // Compute the other two corners
        Vec3 topRight(bottomRight.x, topLeft.y, topLeft.z);
        Vec3 bottomLeft(topLeft.x, bottomRight.y, bottomRight.z);

        // Subdivide the rectangle into two triangles
        triangles.push_back(std::make_shared<Triangle>(topLeft, bottomLeft, topRight, material_ptr));
        triangles.push_back(std::make_shared<Triangle>(bottomLeft, bottomRight, topRight, material_ptr));
    }

    /**
     * @brief Checks if a ray intersects with the rectangle (through its triangles).
     * @param ray The ray to test for intersection.
     * @param t_min The minimum distance for intersection.
     * @param t_max The maximum distance for intersection.
     * @param rec A reference to a HitRecord that will store information about the intersection.
     * @return True if the ray intersects the rectangle, false otherwise.
     */
    virtual bool hit(const Ray &ray, float t_min, float t_max, HitRecord &rec) const override;

    /**
     * @brief Computes the bounding box of the rectangle.
     * @param output_box The AABB to store the bounding box.
     * @return True if the bounding box is successfully computed, false otherwise.
     */
    virtual bool bounding_box(AABB &output_box) const override;

private:
    std::shared_ptr<Material> material_ptr;           ///< The material associated with the rectangle.
    std::vector<std::shared_ptr<Triangle>> triangles; ///< The two triangles that make up the rectangle.
};

#endif // RECTANGLE_H
