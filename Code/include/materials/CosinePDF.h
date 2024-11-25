#ifndef COSINE_PDF_H
#define COSINE_PDF_H

#include "materials/PDF.h"

/**
 * @class CosinePDF
 * @brief Represents a cosine-weighted probability density function (PDF) for sampling directions in 3D space.
 *        This is typically used for materials with Lambertian (diffuse) reflection, where the probability
 *        of a direction is proportional to the cosine of the angle between the surface normal and the direction.
 */
class CosinePDF : public PDF
{
public:
    /**
     * @brief Constructs a CosinePDF with a specified normal direction.
     * @param w The surface normal vector around which the cosine-weighted distribution is defined.
     */
    CosinePDF(const Vec3 &w) : m_normal(w) {}

    float value(const Vec3 &direction) const override;
    Vec3 generate() const override;

private:
    Vec3 m_normal; ///< The surface normal around which the cosine-weighted distribution is defined.
};

#endif // COSINE_PDF_H
