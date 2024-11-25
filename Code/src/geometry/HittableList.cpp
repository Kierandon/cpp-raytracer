#include "geometry/HittableList.h"

bool HittableList::hit(const Ray &ray, float t_min, float t_max, HitRecord &rec) const
{
    HitRecord temp_rec;
    bool hit_anything = false;
    float closest_so_far = t_max;

    for (const auto &object : objects)
    {
        if (object->hit(ray, t_min, closest_so_far, temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

bool HittableList::bounding_box(AABB &output_box) const
{
    if (objects.empty())
        return false;

    AABB temp_box;
    bool first_box = true;

    for (const auto &object : objects)
    {
        if (!object->bounding_box(temp_box))
            return false;
        output_box = first_box ? temp_box : AABB::surrounding_box(output_box, temp_box);
        first_box = false;
    }

    return true;
}
