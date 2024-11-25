#include "materials/CosinePDF.h"

float CosinePDF::value(const Vec3 &direction) const
{
    float cosine = direction.normalized().dot(m_normal);
    return cosine > 0 ? cosine / M_PI : 0;
};

Vec3 CosinePDF::generate() const
{
    return random_cosine_direction(m_normal);
};
