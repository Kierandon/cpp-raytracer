#ifndef TONE_MAPPER_H
#define TONE_MAPPER_H

#include "core/Vec3.h"

/**
 * @class ToneMapper
 * @brief Abstract base class for tone mapping operators used to convert high dynamic range (HDR) colors to low dynamic range (LDR).
 *        Tone mapping is a crucial process in computer graphics and image processing, as it compresses the wide range of HDR colors
 *        into a range that can be displayed on standard monitors (LDR). Derived classes implement specific tone mapping algorithms.
 */
class ToneMapper
{
public:
    /**
     * @brief Virtual destructor for the ToneMapper class.
     */
    virtual ~ToneMapper() = default;

    /**
     * @brief Pure virtual function to map an HDR color to an LDR color using a specific tone mapping operator.
     *        This function must be implemented by derived classes, with each class providing its own tone mapping formula.
     * @param hdr_color The high dynamic range color to be tone-mapped to low dynamic range.
     * @return The tone-mapped low dynamic range color.
     */
    virtual Vec3 map(const Vec3 &hdr_color) const = 0;
};

#endif // TONE_MAPPER_H
