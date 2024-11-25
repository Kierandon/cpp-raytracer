#include "geometry/Triangle.h"

#include <cmath>

bool Triangle::hit(const Ray &ray, float t_min, float t_max, HitRecord &rec) const
{
    // Möller–Trumbore intersection algorithm
    Vec3 edge1 = vertex1 - vertex0;
    Vec3 edge2 = vertex2 - vertex0;
    Vec3 h = ray.direction().cross(edge2);
    float a = edge1.dot(h);

    if (fabs(a) < 1e-6)
        return false;

    float f = 1.0f / a;
    Vec3 s = ray.origin() - vertex0;
    float u = f * s.dot(h);

    if (u < 0.0f || u > 1.0f)
        return false;

    Vec3 q = s.cross(edge1);
    float v = f * ray.direction().dot(q);

    if (v < 0.0f || u + v > 1.0f)
        return false;

    float t = f * edge2.dot(q);
    if (t < t_min || t > t_max)
        return false;

    rec.t = t;
    rec.point = ray.at(t);
    rec.normal = normal;
    rec.set_face_normal(ray, normal);
    rec.material_ptr = material_ptr;

    // Planar mapping: calculate u, v texture coordinates
    // Use edge1 and edge2 to define a local coordinate system
    Vec3 p_local = rec.point - vertex0;                     // Translate to local space
    float u_planar = p_local.dot(edge1) / edge1.dot(edge1); // Project onto edge1
    float v_planar = p_local.dot(edge2) / edge2.dot(edge2); // Project onto edge2

    rec.u = std::max(0.0f, std::min(1.0f, u_planar)); // Map to [0, 1]
    rec.v = std::max(0.0f, std::min(1.0f, v_planar)); // Map to [0, 1]

    return true;
}

bool Triangle::bounding_box(AABB &output_box) const
{
    Vec3 min(
        std::min(std::min(vertex0.x, vertex1.x), vertex2.x),
        std::min(std::min(vertex0.y, vertex1.y), vertex2.y),
        std::min(std::min(vertex0.z, vertex1.z), vertex2.z));
    Vec3 max(
        std::max(std::max(vertex0.x, vertex1.x), vertex2.x),
        std::max(std::max(vertex0.y, vertex1.y), vertex2.y),
        std::max(std::max(vertex0.z, vertex1.z), vertex2.z));

    // Add a small epsilon to avoid zero-size bounding boxes
    const float epsilon = 1e-5f;
    min = min - Vec3(epsilon, epsilon, epsilon);
    max = max + Vec3(epsilon, epsilon, epsilon);

    output_box = AABB(min, max);
    return true;
}
