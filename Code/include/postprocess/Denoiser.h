#ifndef DENOISER_H
#define DENOISER_H

#include "core/Vec3.h"
#include "scene/SceneConfig.h"

#include <vector>

/**
 * @class Denoiser
 * @brief Abstract base class for denoising algorithms.
 *        Denoisers are used to reduce noise in rendered images or pixel arrays. This class provides the
 *        basic interface and common functionality for denoising algorithms, with derived classes implementing
 *        specific denoising techniques (e.g., bilateral filtering, temporal filtering).
 */
class Denoiser
{
public:
    /**
     * @brief Constructs a Denoiser object with a given scene configuration and pixel data.
     * @param config The scene configuration containing parameters for the denoising algorithm.
     * @param pixels A reference to the vector of pixels that will be denoised.
     */
    Denoiser(const SceneConfig &config, std::vector<Vec3> &pixels)
        : config(config), pixels(pixels) {}

    /**
     * @brief Pure virtual function that performs the denoising process.
     *        This function must be implemented by derived classes, as each denoising algorithm may have
     *        different implementation details.
     */
    virtual void denoise() = 0;

protected:
    const SceneConfig &config; ///< The scene configuration, which includes parameters for the denoising process.
    std::vector<Vec3> &pixels; ///< The vector of pixels that will be denoised.
};

#endif // DENOISER_H
