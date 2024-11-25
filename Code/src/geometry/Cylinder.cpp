#include "geometry/Cylinder.h"

#include <cmath>

bool Cylinder::hit(const Ray &ray, float t_min, float t_max, HitRecord &rec) const
{
    Vec3 oc = ray.origin() - center;
    Vec3 ray_dir_proj = ray.direction() - ray.direction().dot(axis) * axis;
    Vec3 oc_proj = oc - oc.dot(axis) * axis;

    float a = ray_dir_proj.dot(ray_dir_proj);
    float b = 2.0f * oc_proj.dot(ray_dir_proj);
    float c = oc_proj.dot(oc_proj) - radius * radius;
    float discriminant = b * b - 4 * a * c;

    float closest_t = t_max;
    bool hit_anything = false;

    // Side intersection
    if (discriminant >= 0)
    {
        float sqrt_disc = sqrt(discriminant);
        float t1 = (-b - sqrt_disc) / (2.0f * a);
        float t2 = (-b + sqrt_disc) / (2.0f * a);

        for (float t : {t1, t2})
        {
            if (t >= t_min && t <= closest_t)
            {
                Vec3 hit_point = ray.at(t);
                float height_proj = (hit_point - center).dot(axis);
                if (std::abs(height_proj) <= height)
                {
                    closest_t = t;
                    hit_anything = true;
                    rec.t = t;
                    rec.point = hit_point;
                    Vec3 axis_point = center + height_proj * axis;
                    rec.normal = (hit_point - axis_point) / radius;
                    rec.set_face_normal(ray, rec.normal);

                    // Compute texture coordinates for the side
                    Vec3 from_axis = hit_point - axis_point;
                    float theta = atan2(from_axis.z, from_axis.x);
                    rec.u = (theta + M_PI) / (2 * M_PI);
                    rec.v = (height_proj + height) / (2 * height); // Map height to [0, 1]
                }
            }
        }
    }

    // Cap intersections
    float denom = ray.direction().dot(axis);
    if (std::abs(denom) > 1e-6)
    {
        // Bottom cap
        Vec3 bottom_center = center - axis * height;
        float t = (bottom_center - ray.origin()).dot(axis) / denom;
        if (t >= t_min && t <= closest_t)
        {
            Vec3 hit_point = ray.at(t);
            Vec3 from_center = hit_point - bottom_center;
            float r2 = from_center.dot(from_center) - pow(from_center.dot(axis), 2);
            if (r2 <= radius * radius)
            {
                closest_t = t;
                hit_anything = true;
                rec.t = t;
                rec.point = hit_point;
                rec.normal = -axis;
                rec.set_face_normal(ray, rec.normal);

                // Compute texture coordinates for the bottom cap
                Vec3 cap_relative = hit_point - bottom_center;
                rec.u = (cap_relative.x + radius) / (2 * radius);
                rec.v = (cap_relative.z + radius) / (2 * radius);
            }
        }

        // Top cap
        Vec3 top_center = center + axis * height;
        t = (top_center - ray.origin()).dot(axis) / denom;
        if (t >= t_min && t <= closest_t)
        {
            Vec3 hit_point = ray.at(t);
            Vec3 from_center = hit_point - top_center;
            float r2 = from_center.dot(from_center) - pow(from_center.dot(axis), 2);
            if (r2 <= radius * radius)
            {
                closest_t = t;
                hit_anything = true;
                rec.t = t;
                rec.point = hit_point;
                rec.normal = axis;
                rec.set_face_normal(ray, rec.normal);

                // Compute texture coordinates for the top cap
                Vec3 cap_relative = hit_point - top_center;
                rec.u = (cap_relative.x + radius) / (2 * radius);
                rec.v = (cap_relative.z + radius) / (2 * radius);
            }
        }
    }

    if (hit_anything)
    {
        rec.material_ptr = material_ptr;
        return true;
    }

    return false;
}

bool Cylinder::bounding_box(AABB &output_box) const
{
    const float padding = 0.0001f; // Small padding to avoid floating point precision issues

    // Get the end points of the cylinder axis
    Vec3 bottom_center = center - axis * height;
    Vec3 top_center = center + axis * height;

    // Find the minimum coordinates
    Vec3 min_point(
        std::min(bottom_center.x, top_center.x) - radius,
        std::min(bottom_center.y, top_center.y) - radius,
        std::min(bottom_center.z, top_center.z) - radius);

    // Find the maximum coordinates
    Vec3 max_point(
        std::max(bottom_center.x, top_center.x) + radius,
        std::max(bottom_center.y, top_center.y) + radius,
        std::max(bottom_center.z, top_center.z) + radius);

    // Add padding
    min_point = min_point - Vec3(padding);
    max_point = max_point + Vec3(padding);

    output_box = AABB(min_point, max_point);
    return true;
}
