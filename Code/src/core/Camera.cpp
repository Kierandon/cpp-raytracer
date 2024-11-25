#include "core/Camera.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>

#include "core/Utils.h"

Camera::Camera(Vec3 lookfrom, Vec3 lookat, Vec3 vup,
               float vfov, float aspect_ratio,
               float aperture, float focus_dist,
               bool enable_dof)
{
    float theta = vfov * M_PI / 180.0f;
    float h = std::tan(theta / 2);
    float viewport_height = 2.0f * h;
    float viewport_width = aspect_ratio * viewport_height;

    w = (lookfrom - lookat).normalized();
    u = w.cross(vup).normalized();
    v = u.cross(w);

    origin = lookfrom;
    horizontal = focus_dist * viewport_width * u;
    vertical = focus_dist * viewport_height * v;
    lower_left = origin - horizontal / 2 - vertical / 2 - focus_dist * w;
    use_dof = enable_dof;

    lens_radius = aperture / 2;
}

Vec3 Camera::random_in_unit_disk() const
{
    while (true)
    {
        Vec3 p(random_float(-1, 1), random_float(-1, 1), 0);
        if (p.dot(p) < 1)
            return p;
    }
}

Ray Camera::get_ray(float s, float t) const
{
    if (!use_dof)
    {
        // Pinhole camera - no lens sampling
        return Ray(origin,
                   lower_left + s * horizontal + t * vertical - origin);
    }

    // DoF enabled - use lens sampling
    Vec3 rd = lens_radius * random_in_unit_disk();
    Vec3 offset = u * rd.x + v * rd.y;
    Vec3 ray_origin = origin + offset;
    return Ray(ray_origin,
               lower_left + s * horizontal + t * vertical - ray_origin);
}
