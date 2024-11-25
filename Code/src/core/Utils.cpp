#include "core/Utils.h"
#include "core/Vec3.h"

#include <cmath>
#include <cstdlib>
#include <random>

float random_float()
{
    static std::mt19937 generator(std::random_device{}());
    static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
    return distribution(generator);
}

float random_float(float min, float max)
{
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

Vec3 reflect(const Vec3 &v, const Vec3 &n)
{
    return v - 2 * v.dot(n) * n;
}

Vec3 refract(const Vec3 &v, const Vec3 &n, float eta)
{
    float cos_theta = std::min(-v.dot(n), 1.0f);
    Vec3 r_out_perp = eta * (v + cos_theta * n);
    float k = 1.0f - r_out_perp.dot(r_out_perp);
    if (k < 0.0f)
    {
        return Vec3(0.0f, 0.0f, 0.0f);
    }
    else
    {
        Vec3 r_out_parallel = -std::sqrt(k) * n;
        return r_out_perp + r_out_parallel;
    }
}

float schlick(float cosine, float ref_idx)
{
    float r0 = (1.0f - ref_idx) / (1.0f + ref_idx);
    r0 = r0 * r0;
    return r0 + (1.0f - r0) * std::pow(1.0f - cosine, 5);
}

Vec3 random_unit_vector()
{
    float a = random_float(0, 2 * M_PI);
    float z = random_float(-1, 1);
    float r = std::sqrt(1 - z * z);
    return Vec3(r * std::cos(a), r * std::sin(a), z);
}

Vec3 random_point_in_unit_sphere()
{
    while (true)
    {
        // Generate random x, y, z between -1 and 1
        float x = static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f;
        float y = static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f;
        float z = static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f;

        Vec3 point(x, y, z);

        // Check if the point lies inside the unit sphere
        if (point.length() * point.length() <= 1.0f)
        {
            return point;
        }
    }
}

Vec3 random_cosine_direction(const Vec3 &normal)
{
    // Generate random numbers for polar coordinates
    float r1 = static_cast<float>(rand()) / RAND_MAX;
    float r2 = static_cast<float>(rand()) / RAND_MAX;

    // Convert to spherical coordinates (using concentric disk mapping)
    float phi = 2.0f * M_PI * r1;
    float cos_theta = std::sqrt(r2); // This gives us cosine-weighted sampling
    float sin_theta = std::sqrt(1.0f - r2);

    // Convert to Cartesian coordinates on unit sphere
    float x = std::cos(phi) * sin_theta;
    float y = std::sin(phi) * sin_theta;
    float z = cos_theta;

    // Create an orthonormal basis aligned with the normal
    Vec3 w = normal;
    Vec3 a = (std::fabs(w.x) > 0.9f) ? Vec3(0, 1, 0) : Vec3(1, 0, 0);
    Vec3 v = w.cross(a).normalized();
    Vec3 u = w.cross(v);

    // Transform from local to world coordinates
    return (u * x + v * y + w * z).normalized();
}
