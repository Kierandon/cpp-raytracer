#include "core/Ray.h"

// Constructors
Ray::Ray() : orig(Vec3()), dir(Vec3(1, 0, 0)) {}

Ray::Ray(const Vec3 &origin, const Vec3 &direction) : orig(origin), dir(direction) {}

// Accessors
Vec3 Ray::origin() const
{
    return orig;
}

Vec3 Ray::direction() const
{
    return dir;
}

// Compute a point along the ray
Vec3 Ray::at(float t) const
{
    return orig + t * dir;
}
