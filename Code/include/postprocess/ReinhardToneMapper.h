#ifndef REINHARD_TONE_MAPPER_H
#define REINHARD_TONE_MAPPER_H

#include "postprocess/ToneMapper.h"

/**
 * @class ReinhardToneMapper
 * @brief Implements the Reinhard tone mapping operator for high dynamic range (HDR) to low dynamic range (LDR) conversion.
 *        The Reinhard tone mapping operator is a simple, global operator that compresses the dynamic range of HDR images
 *        by using a sigmoid function. It is commonly used in image processing and rendering to produce more visually
 *        pleasing results when mapping HDR colors to displayable LDR values.
 */
class ReinhardToneMapper : public ToneMapper
{
public:
    /**
     * @brief Maps an HDR color to an LDR color using the Reinhard tone mapping operator.
     *        This function reduces the dynamic range of the input HDR color and returns a mapped LDR color.
     *        The operator is defined as `LDR = HDR / (HDR + 1)`, where HDR is the high dynamic range input color.
     * @param hdr_color The high dynamic range color that needs to be tone-mapped to a low dynamic range.
     * @return The tone-mapped low dynamic range color.
     */
    Vec3 map(const Vec3 &hdr_color) const override
    {
        return hdr_color / (hdr_color + Vec3(1, 1, 1)); ///< Apply the Reinhard tone mapping formula.
    }
};

#endif // REINHARD_TONE_MAPPER_H
