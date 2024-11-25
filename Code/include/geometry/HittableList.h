#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "geometry/Hittable.h"
#include <vector>
#include <memory>

/**
 * @class HittableList
 * @brief Represents a collection of hittable objects, typically used to manage and perform ray intersection tests on a group of objects.
 *        It allows adding, clearing, and checking intersections with multiple objects at once.
 */
class HittableList : public Hittable
{
public:
    /**
     * @brief Default constructor for an empty HittableList.
     */
    HittableList() {}

    /**
     * @brief Constructs a HittableList with a single hittable object.
     * @param object The hittable object to add to the list.
     */
    HittableList(std::shared_ptr<Hittable> object) { add(object); }

    /**
     * @brief Adds a new hittable object to the list.
     * @param object A shared pointer to the hittable object to add.
     */
    void add(std::shared_ptr<Hittable> object) { objects.push_back(object); }

    /**
     * @brief Clears all objects from the list.
     */
    void clear() { objects.clear(); }

    /**
     * @brief Checks if a ray intersects with any object in the list.
     * @param ray The ray to test for intersection.
     * @param t_min The minimum distance for intersection.
     * @param t_max The maximum distance for intersection.
     * @param rec A reference to a HitRecord that will store information about the intersection.
     * @return True if the ray intersects any object in the list, false otherwise.
     */
    virtual bool hit(const Ray &ray, float t_min, float t_max, HitRecord &rec) const override;

    /**
     * @brief Computes the bounding box of all the objects in the list.
     * @param output_box The AABB to store the bounding box of the list.
     * @return True if the bounding box is successfully computed, false otherwise.
     */
    virtual bool bounding_box(AABB &output_box) const override;

    /**
     * @brief A vector of shared pointers to the hittable objects in the list.
     */
    std::vector<std::shared_ptr<Hittable>> objects;
};

#endif // HITTABLE_LIST_H
