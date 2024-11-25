#ifndef HIT_RECORD_H
#define HIT_RECORD_H

#include "core/Vec3.h"
#include "core/Ray.h"
#include <memory>

class Material;

// A record to store details of an intersection
/**
 * @struct HitRecord
 * @brief A structure that stores the details of a ray-object intersection.
 *        This includes the intersection point, normal, material, and other relevant information
 *        such as the ray's direction and texture coordinates.
 */
struct HitRecord
{
    Vec3 point;                             ///< The point of intersection where the ray hits the object.
    Vec3 normal;                            ///< The normal vector at the point of intersection.
    float t;                                ///< The distance along the ray to the intersection point.
    float u;                                ///< The U texture coordinate (if available).
    float v;                                ///< The V texture coordinate (if available).
    bool front_face;                        ///< Indicates whether the intersection is on the front face of the object.
    std::shared_ptr<Material> material_ptr; ///< The material associated with the intersected object.

    /**
     * @brief Sets the normal for the intersection point based on the ray's direction.
     *        The front face is determined by checking if the ray direction is facing the outward normal.
     * @param ray The ray that intersected the object.
     * @param outward_normal The normal vector pointing outward from the object.
     */
    void set_face_normal(const Ray &ray, const Vec3 &outward_normal)
    {
        front_face = ray.direction().dot(outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

#endif // HIT_RECORD_H
