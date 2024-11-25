#ifndef BVH_NODE_H
#define BVH_NODE_H

#include "geometry/Hittable.h"
#include "geometry/AABB.h"
#include <vector>
#include <memory>

/**
 * @class BVHNode
 * @brief Represents a node in a Bounding Volume Hierarchy (BVH) for efficient ray tracing.
 *        A BVH is a tree structure where each node contains a bounding box and references
 *        to two child nodes (left and right), which contain the actual geometric objects.
 */
class BVHNode : public Hittable
{
public:
    /**
     * @brief Constructs a BVHNode with a list of hittable objects, subdividing the objects into left and right nodes.
     * @param objects The list of hittable objects to divide.
     * @param start The starting index in the list of objects.
     * @param end The ending index in the list of objects.
     */
    BVHNode(const std::vector<std::shared_ptr<Hittable>> &objects, size_t start, size_t end);

    /**
     * @brief Checks if a ray intersects with the objects within this BVHNode.
     * @param ray The ray to test for intersection.
     * @param t_min The minimum distance for intersection.
     * @param t_max The maximum distance for intersection.
     * @param rec A reference to a HitRecord that will store information about the intersection.
     * @return True if the ray intersects any object in the node, false otherwise.
     */
    virtual bool hit(const Ray &ray, float t_min, float t_max, HitRecord &rec) const override;

    /**
     * @brief Computes the bounding box of this BVHNode.
     * @param output_box The AABB to store the bounding box.
     * @return True if the bounding box is successfully computed, false otherwise.
     */
    virtual bool bounding_box(AABB &output_box) const override;

private:
    std::shared_ptr<Hittable> left;  ///< Left child node containing a sublist of objects.
    std::shared_ptr<Hittable> right; ///< Right child node containing a sublist of objects.
    AABB box;                        ///< The bounding box of the node that contains both child nodes.

    /**
     * @brief Compares two hittable objects based on their bounding boxes along a given axis.
     * @param a The first hittable object.
     * @param b The second hittable object.
     * @param axis The axis (0: x-axis, 1: y-axis, 2: z-axis) to compare along.
     * @return True if the bounding box of object a is smaller than that of object b along the given axis, false otherwise.
     */
    static bool box_compare(const std::shared_ptr<Hittable> a,
                            const std::shared_ptr<Hittable> b, int axis);

    /**
     * @brief Compares two hittable objects based on their bounding boxes along the x-axis.
     * @param a The first hittable object.
     * @param b The second hittable object.
     * @return True if the bounding box of object a is smaller than that of object b along the x-axis, false otherwise.
     */
    static bool box_x_compare(const std::shared_ptr<Hittable> a,
                              const std::shared_ptr<Hittable> b);

    /**
     * @brief Compares two hittable objects based on their bounding boxes along the y-axis.
     * @param a The first hittable object.
     * @param b The second hittable object.
     * @return True if the bounding box of object a is smaller than that of object b along the y-axis, false otherwise.
     */
    static bool box_y_compare(const std::shared_ptr<Hittable> a,
                              const std::shared_ptr<Hittable> b);

    /**
     * @brief Compares two hittable objects based on their bounding boxes along the z-axis.
     * @param a The first hittable object.
     * @param b The second hittable object.
     * @return True if the bounding box of object a is smaller than that of object b along the z-axis, false otherwise.
     */
    static bool box_z_compare(const std::shared_ptr<Hittable> a,
                              const std::shared_ptr<Hittable> b);
};

#endif
