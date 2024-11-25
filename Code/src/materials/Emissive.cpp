#include "materials/Emissive.h"

bool Emissive::scatter([[maybe_unused]] const Ray &ray_in,
                       [[maybe_unused]] const HitRecord &rec,
                       [[maybe_unused]] ScatterRecord &scatter_rec) const
{
    return false; // Emissive materials don't scatter
}

Vec3 Emissive::brdf([[maybe_unused]] const HitRecord &rec,
                    [[maybe_unused]] const Vec3 &view_dir,
                    [[maybe_unused]] const Vec3 &light_dir) const
{
    return Vec3(0, 0, 0);
}

Vec3 Emissive::emitted([[maybe_unused]] const HitRecord &rec,
                       [[maybe_unused]] float u,
                       [[maybe_unused]] float v,
                       [[maybe_unused]] const Vec3 &p) const
{
    return m_color * m_intensity;
}
