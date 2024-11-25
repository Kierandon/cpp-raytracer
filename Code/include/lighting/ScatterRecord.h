#ifndef SCATTER_RECORD_H
#define SCATTER_RECORD_H

#include "core/Vec3.h"
#include "materials/PDF.h"
#include <memory>

/**
 * @struct ScatterRecord
 * @brief A structure that stores information about a scattered ray in a material interaction.
 *        This includes the attenuation of the ray, the probability density function (PDF) for scattering,
 *        and information about whether the scattered ray is specular.
 */
struct ScatterRecord
{
    Vec3 attenuation;             ///< The attenuation factor for the scattered ray, representing how much light is absorbed or scattered.
    std::shared_ptr<PDF> pdf_ptr; ///< A pointer to the PDF that describes the scattering probability distribution.
    bool specular_ray = false;    ///< Indicates if the scattered ray is specular (i.e., perfectly reflected).
    Vec3 specular_direction;      ///< The direction of the specular ray if the ray is specular.
};

#endif // SCATTER_RECORD_H
