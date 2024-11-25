#include "materials/Dielectric.h"

bool Dielectric::scatter(const Ray &ray_in,
                         const HitRecord &rec,
                         ScatterRecord &scatter_rec) const
{
    scatter_rec.specular_ray = true;
    scatter_rec.attenuation = Vec3(1, 1, 1);

    float refraction_ratio = rec.front_face ? (1.0f / m_ior) : m_ior;
    Vec3 unit_direction = ray_in.direction().normalized();

    float cos_theta = std::min(-unit_direction.dot(rec.normal), 1.0f);
    float sin_theta = std::sqrt(1.0f - cos_theta * cos_theta);

    bool cannot_refract = refraction_ratio * sin_theta > 1.0f;
    Vec3 direction;

    if (cannot_refract || schlick(cos_theta, refraction_ratio) > random_float())
    {
        direction = reflect(unit_direction, rec.normal);
    }
    else
    {
        direction = refract(unit_direction, rec.normal, refraction_ratio);
    }

    scatter_rec.specular_direction = direction;
    return true;
};

Vec3 Dielectric::brdf([[maybe_unused]] const HitRecord &rec,
                      [[maybe_unused]] const Vec3 &view_dir,
                      [[maybe_unused]] const Vec3 &light_dir) const
{
    return Vec3(0, 0, 0); // Not used for dielectrics
};
