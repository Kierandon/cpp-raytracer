#include "geometry/Rectangle.h"

bool Rectangle::hit(const Ray &ray, float t_min, float t_max, HitRecord &rec) const
{
    HitRecord temp_rec;
    bool hit_anything = false;
    float closest_so_far = t_max;

    for (const auto &triangle : triangles)
    {
        if (triangle->hit(ray, t_min, closest_so_far, temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
            closest_so_far = rec.t;
        }
    }

    return hit_anything;
}

bool Rectangle::bounding_box(AABB &output_box) const
{
    AABB box0, box1;

    if (!triangles[0]->bounding_box(box0) || !triangles[1]->bounding_box(box1))
        return false;

    Vec3 small(fmin(box0.minimum.x, box1.minimum.x),
               fmin(box0.minimum.y, box1.minimum.y),
               fmin(box0.minimum.z, box1.minimum.z));

    Vec3 big(fmax(box0.maximum.x, box1.maximum.x),
             fmax(box0.maximum.y, box1.maximum.y),
             fmax(box0.maximum.z, box1.maximum.z));

    // Add a small padding to prevent zero-thickness boxes
    const float padding = 0.0001f;
    if (abs(big.x - small.x) < padding)
    {
        small.x -= padding;
        big.x += padding;
    }
    if (abs(big.y - small.y) < padding)
    {
        small.y -= padding;
        big.y += padding;
    }
    if (abs(big.z - small.z) < padding)
    {
        small.z -= padding;
        big.z += padding;
    }

    output_box = AABB(small, big);
    return true;
}
