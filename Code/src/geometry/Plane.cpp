#include "geometry/Plane.h"

#include <cmath>

bool Plane::hit(const Ray &ray, float t_min, float t_max, HitRecord &rec) const
{
    float denom = normal.dot(ray.direction());

    // Check if ray is parallel to plane
    if (fabs(denom) < 1e-6)
        return false;

    float t = (point - ray.origin()).dot(normal) / denom;

    if (t < t_min || t > t_max)
        return false;

    rec.t = t;
    rec.point = ray.at(t);
    rec.set_face_normal(ray, normal);
    rec.material_ptr = material_ptr;

    return true;
}

bool Plane::bounding_box(AABB &output_box) const
{
    // Planes are infinite, need special handling
    // Create a large but finite box
    float huge = 1000000.0f;
    Vec3 min_point = Vec3(-huge, -0.001f, -huge); // Small thickness in normal direction
    Vec3 max_point = Vec3(huge, 0.001f, huge);

    // Transform based on plane orientation
    if (normal.y < 0.99f)
    {
        // Handle non-horizontal planes
        min_point = Vec3(-huge, -huge, -huge);
        max_point = Vec3(huge, huge, huge);
    }

    output_box = AABB(min_point, max_point);
    return true;
}
