#include "geometry/Box.h"



Box::Box(const Vec3 &min, const Vec3 &max, const Vec3 &rotation_deg, std::shared_ptr<Material> mat)
    : min(min), max(max), rotation(rotation_deg * M_PI / 180.0f), material_ptr(mat)
{
    Vec3 v0 = min;
    Vec3 v1 = Vec3(max.x, min.y, min.z);
    Vec3 v2 = Vec3(max.x, max.y, min.z);
    Vec3 v3 = Vec3(min.x, max.y, min.z);
    Vec3 v4 = Vec3(min.x, min.y, max.z);
    Vec3 v5 = Vec3(max.x, min.y, max.z);
    Vec3 v6 = max;
    Vec3 v7 = Vec3(min.x, max.y, max.z);

    v0 = rotatePoint(v0);
    v1 = rotatePoint(v1);
    v2 = rotatePoint(v2);
    v3 = rotatePoint(v3);
    v4 = rotatePoint(v4);
    v5 = rotatePoint(v5);
    v6 = rotatePoint(v6);
    v7 = rotatePoint(v7);

    triangles.push_back(std::make_shared<Triangle>(v0, v1, v2, material_ptr));
    triangles.push_back(std::make_shared<Triangle>(v0, v2, v3, material_ptr));
    triangles.push_back(std::make_shared<Triangle>(v1, v5, v6, material_ptr));
    triangles.push_back(std::make_shared<Triangle>(v1, v6, v2, material_ptr));
    triangles.push_back(std::make_shared<Triangle>(v5, v4, v7, material_ptr));
    triangles.push_back(std::make_shared<Triangle>(v5, v7, v6, material_ptr));
    triangles.push_back(std::make_shared<Triangle>(v4, v0, v3, material_ptr));
    triangles.push_back(std::make_shared<Triangle>(v4, v3, v7, material_ptr));
    triangles.push_back(std::make_shared<Triangle>(v3, v2, v6, material_ptr));
    triangles.push_back(std::make_shared<Triangle>(v3, v6, v7, material_ptr));
    triangles.push_back(std::make_shared<Triangle>(v4, v5, v1, material_ptr));
    triangles.push_back(std::make_shared<Triangle>(v4, v1, v0, material_ptr));
}

Vec3 Box::rotatePoint(const Vec3& point) const 
{
    Vec3 rotated = point;
    
    // Rotate around X axis
    if (rotation.x != 0) {
        float cosX = cos(rotation.x);
        float sinX = sin(rotation.x);
        float y = rotated.y;
        float z = rotated.z;
        rotated.y = y * cosX - z * sinX;
        rotated.z = y * sinX + z * cosX;
    }
    
    // Rotate around Y axis
    if (rotation.y != 0) {
        float cosY = cos(rotation.y);
        float sinY = sin(rotation.y);
        float x = rotated.x;
        float z = rotated.z;
        rotated.x = x * cosY + z * sinY;
        rotated.z = -x * sinY + z * cosY;
    }
    
    // Rotate around Z axis
    if (rotation.z != 0) {
        float cosZ = cos(rotation.z);
        float sinZ = sin(rotation.z);
        float x = rotated.x;
        float y = rotated.y;
        rotated.x = x * cosZ - y * sinZ;
        rotated.y = x * sinZ + y * cosZ;
    }
    
    return rotated;
}

bool Box::hit(const Ray &ray, float t_min, float t_max, HitRecord &rec) const
{
    HitRecord temp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;

    for (const auto &triangle : triangles)
    {
        if (triangle->hit(ray, t_min, closest_so_far, temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

bool Box::bounding_box(AABB &output_box) const
{
    output_box = AABB(min, max);
    return true;
}
