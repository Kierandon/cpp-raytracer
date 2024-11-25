#ifndef BILATERALDENOISER_H
#define BILATERALDENOISER_H

#include "postprocess/Denoiser.h"

/**
 * @class BilateralDenoiser
 * @brief Implements a bilateral denoising algorithm for image noise reduction.
 *        The bilateral filter is a non-linear filter that smooths images while preserving edges.
 *        It uses both spatial and range (color) information to determine the amount of smoothing to apply.
 *        This class is designed for use with images or pixel arrays where denoising is needed.
 */
class BilateralDenoiser : public Denoiser
{
public:
    /**
     * @brief Constructs a BilateralDenoiser with a given scene configuration and pixel data.
     * @param config The scene configuration containing the parameters for the bilateral filter, such as sigma values.
     * @param pixels A reference to the vector of pixels that will be denoised.
     */
    BilateralDenoiser(const SceneConfig &config, std::vector<Vec3> &pixels)
        : Denoiser(config, pixels)
    {
        sigmaSpatial = config.bilateral_sigma_spatial; ///< Spatial sigma controls the distance-based influence.
        sigmaRange = config.bilateral_sigma_range;     ///< Range sigma controls the color-based influence.
    }

    /**
     * @brief Performs the bilateral denoising process on the pixel data.
     *        This function applies the bilateral filter, which uses both the spatial and range
     *        sigma values to filter noise while preserving edges in the image.
     */
    void denoise() override;

private:
    float sigmaSpatial; ///< The spatial sigma parameter, which controls the amount of smoothing based on pixel distance.
    float sigmaRange;   ///< The range sigma parameter, which controls the amount of smoothing based on pixel color difference.
};

#endif // BILATERALDENOISER_H
