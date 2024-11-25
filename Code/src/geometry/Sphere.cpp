#include "geometry/Sphere.h"
#include <cmath>

bool Sphere::hit(const Ray &ray, float t_min, float t_max, HitRecord &rec) const
{
    Vec3 oc = ray.origin() - centre;
    float a = ray.direction().dot(ray.direction());
    float b = 2.0 * oc.dot(ray.direction());
    float c = oc.dot(oc) - radius * radius;

    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
    {
        return false; // No intersection
    }

    float sqrt_discriminant = std::sqrt(discriminant);

    float root = (-b - sqrt_discriminant) / (2.0 * a);
    if (root < t_min || root > t_max)
    {
        root = (-b + sqrt_discriminant) / (2.0 * a);
        if (root < t_min || root > t_max)
        {
            return false; // No valid roots
        }
    }

    rec.t = root;
    rec.point = ray.at(rec.t);
    Vec3 outward_normal = (rec.point - centre) / radius;
    rec.set_face_normal(ray, outward_normal);
    rec.material_ptr = material_ptr;

    // Compute texture coordinates
    Vec3 p = (rec.point - centre).normalized(); // Normalized to sphere surface
    float theta = acos(-p.y);                   // Angle from pole (vertical)
    float phi = atan2(-p.z, p.x) + M_PI;        // Angle around Y-axis (horizontal)
    rec.u = phi / (2 * M_PI);                   // Map phi to [0, 1]
    rec.v = theta / M_PI;                       // Map theta to [0, 1]

    return true;
}

bool Sphere::bounding_box(AABB &output_box) const
{
    output_box = AABB(
        centre - Vec3(radius),
        centre + Vec3(radius));
    return true;
}
