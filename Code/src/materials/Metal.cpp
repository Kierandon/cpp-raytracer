#include "materials/Metal.h"

bool Metal::scatter(const Ray &ray_in,
                    const HitRecord &rec,
                    ScatterRecord &scatter_rec) const
{
    Vec3 reflected = reflect(ray_in.direction().normalized(), rec.normal);

    // Add roughness via microfacet-like perturbation
    reflected += m_roughness * random_point_in_unit_sphere();

    scatter_rec.specular_ray = true;
    scatter_rec.specular_direction = reflected;
    scatter_rec.attenuation = m_albedo;
    return reflected.dot(rec.normal) > 0;
}

Vec3 Metal::brdf(const HitRecord &rec,
                 const Vec3 &view_dir,
                 const Vec3 &light_dir) const
{
    // Simplified microfacet-like reflection
    Vec3 half_vector = (view_dir + light_dir).normalized();
    float NdotH = rec.normal.dot(half_vector);
    return m_albedo * std::pow(std::max(0.0f, NdotH), 1.0f / m_roughness);
}
