#include "materials/Diffuse.h"

bool Diffuse::scatter([[maybe_unused]] const Ray &ray_in,
                      const HitRecord &rec,
                      ScatterRecord &scatter_rec) const
{
    // Cosine-weighted hemisphere sampling
    Vec3 scatter_direction = rec.normal + random_unit_vector();
    if (scatter_direction.near_zero())
    {
        scatter_direction = rec.normal;
    }
    scatter_rec.specular_ray = false;
    scatter_rec.attenuation = m_albedo;
    scatter_rec.pdf_ptr = std::make_shared<CosinePDF>(rec.normal);
    return true;
}

Vec3 Diffuse::brdf([[maybe_unused]] const HitRecord &rec,
                   [[maybe_unused]] const Vec3 &view_dir,
                   [[maybe_unused]] const Vec3 &light_dir) const
{
    return m_albedo / M_PI; // Lambertian BRDF
}
