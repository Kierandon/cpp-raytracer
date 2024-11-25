#include "geometry/BVHNode.h"
#include <algorithm>

BVHNode::BVHNode(const std::vector<std::shared_ptr<Hittable>> &objects,
                 size_t start, size_t end)
{
    auto objs = objects;

    int axis = rand() % 3;
    auto comparator = (axis == 0)   ? box_x_compare
                      : (axis == 1) ? box_y_compare
                                    : box_z_compare;

    size_t object_span = end - start;

    if (object_span == 1)
    {
        left = right = objs[start];
    }
    else if (object_span == 2)
    {
        if (comparator(objs[start], objs[start + 1]))
        {
            left = objs[start];
            right = objs[start + 1];
        }
        else
        {
            left = objs[start + 1];
            right = objs[start];
        }
    }
    else
    {
        std::sort(objs.begin() + start, objs.begin() + end, comparator);

        auto mid = start + object_span / 2;
        left = std::make_shared<BVHNode>(objs, start, mid);
        right = std::make_shared<BVHNode>(objs, mid, end);
    }

    AABB box_left, box_right;
    if (!left->bounding_box(box_left) || !right->bounding_box(box_right))
        std::cerr << "No bounding box in BVHNode constructor.\n";

    box = AABB::surrounding_box(box_left, box_right);
}

bool BVHNode::hit(const Ray &ray, float t_min, float t_max, HitRecord &rec) const
{
    if (!box.hit(ray, t_min, t_max))
        return false;

    bool hit_left = left->hit(ray, t_min, t_max, rec);
    bool hit_right = right->hit(ray, t_min, hit_left ? rec.t : t_max, rec);

    return hit_left || hit_right;
}

bool BVHNode::bounding_box(AABB &output_box) const
{
    output_box = box;
    return true;
}

bool BVHNode::box_compare(const std::shared_ptr<Hittable> a,
                          const std::shared_ptr<Hittable> b, int axis)
{
    AABB box_a, box_b;
    if (!a->bounding_box(box_a) || !b->bounding_box(box_b))
        std::cerr << "No bounding box in BVH node constructor.\n";

    return box_a.minimum[axis] < box_b.minimum[axis];
}

bool BVHNode::box_x_compare(const std::shared_ptr<Hittable> a,
                            const std::shared_ptr<Hittable> b)
{
    return box_compare(a, b, 0);
}

bool BVHNode::box_y_compare(const std::shared_ptr<Hittable> a,
                            const std::shared_ptr<Hittable> b)
{
    return box_compare(a, b, 1);
}

bool BVHNode::box_z_compare(const std::shared_ptr<Hittable> a,
                            const std::shared_ptr<Hittable> b)
{
    return box_compare(a, b, 2);
}
