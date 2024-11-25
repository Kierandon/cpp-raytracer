#ifndef PDF_H
#define PDF_H

#include "core/Vec3.h"
#include "core/Utils.h"

/**
 * @class PDF
 * @brief Abstract base class for Probability Density Functions (PDFs) used in material scattering and light sampling.
 *        The PDF defines how directions are sampled probabilistically, such as in Lambertian reflection or other material models.
 *        Derived classes implement specific sampling methods based on the type of material or scattering model.
 */
class PDF
{
public:
    /**
     * @brief Virtual destructor for the PDF class.
     */
    virtual ~PDF() = default;

    /**
     * @brief Calculates the value of the PDF for a given direction.
     *        This function returns the probability density of the direction based on the distribution defined by the subclass.
     * @param direction The direction for which the PDF value is calculated.
     * @return The PDF value for the specified direction.
     */
    virtual float value(const Vec3 &direction) const = 0;

    /**
     * @brief Generates a random direction based on the probability distribution defined by the subclass.
     *        This function samples a direction according to the PDF and returns it as a normalized vector.
     * @return A randomly generated direction based on the PDF's distribution.
     */
    virtual Vec3 generate() const = 0;
};

#endif // PDF_H
