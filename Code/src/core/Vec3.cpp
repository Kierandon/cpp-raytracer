#include "core/Vec3.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include "core/Utils.h"

// Constructors
Vec3::Vec3(float v) : x(v), y(v), z(v) {}
Vec3::Vec3() : x(0), y(0), z(0) {}
Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

// Operators
Vec3 Vec3::operator-() const
{
    return Vec3(-x, -y, -z);
}

Vec3 Vec3::operator+(const Vec3 &v) const
{
    return Vec3(x + v.x, y + v.y, z + v.z);
}

Vec3 &Vec3::operator+=(const Vec3 &v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

Vec3 Vec3::operator-(const Vec3 &v) const
{
    return Vec3(x - v.x, y - v.y, z - v.z);
}

Vec3 Vec3::operator*(float t) const
{
    return Vec3(x * t, y * t, z * t);
}

Vec3 Vec3::operator*(const Vec3 &v) const
{
    return Vec3(x * v.x, y * v.y, z * v.z);
}

Vec3 Vec3::operator/(float t) const
{
    return *this * (1 / t);
}

Vec3 Vec3::operator/(const Vec3 &v) const
{
    return Vec3(x / v.x, y / v.y, z / v.z);
}

Vec3 &Vec3::operator/=(const float t)
{
    x /= t;
    y /= t;
    z /= t;
    return *this;
}

// Utility functions
float Vec3::length() const
{
    return std::sqrt(x * x + y * y + z * z);
}

Vec3 Vec3::normalized() const
{
    return *this / length();
}

// Dot and Cross Product
float Vec3::dot(const Vec3 &v) const
{
    return x * v.x + y * v.y + z * v.z;
}

Vec3 Vec3::cross(const Vec3 &v) const
{
    return Vec3(
        y * v.z - z * v.y,
        z * v.x - x * v.z,
        x * v.y - y * v.x);
}

// Friend functions
std::ostream &operator<<(std::ostream &out, const Vec3 &v)
{
    return out << v.x << " " << v.y << " " << v.z;
}

Vec3 operator*(float t, const Vec3 &v)
{
    return Vec3(v.x * t, v.y * t, v.z * t);
}

Vec3 Vec3::min(const Vec3 &other) const
{
    return Vec3(
        std::min(x, other.x),
        std::min(y, other.y),
        std::min(z, other.z));
}

Vec3 Vec3::max(const Vec3 &other) const
{
    return Vec3(
        std::max(x, other.x),
        std::max(y, other.y),
        std::max(z, other.z));
}

float Vec3::operator[](int i) const
{
    if (i == 0)
        return x;
    if (i == 1)
        return y;
    return z;
}

bool Vec3::near_zero() const
{
    const float threshold = 1e-8; // Adjust as needed
    return fabs(x) < threshold && fabs(y) < threshold && fabs(z) < threshold;
}
