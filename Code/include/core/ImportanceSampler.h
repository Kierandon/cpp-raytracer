#ifndef IMPORTANCE_SAMPLER_H
#define IMPORTANCE_SAMPLER_H

#include "core/Vec3.h"
#include "scene/SceneConfig.h"
#include <vector>

/**
 * @class ImportanceSampler
 * @brief A class for performing importance sampling in a ray tracing scene.
 *
 * The ImportanceSampler class is responsible for calculating the importance of
 * samples in a scene based on their luminance and other factors. It helps in
 * optimizing the rendering process by focusing more samples on important areas.
 *
 * @param config The configuration of the scene.
 * @param min_samples The minimum number of samples to be taken.
 * @param max_samples The maximum number of samples to be taken.
 *
 */
class ImportanceSampler
{
public:
    ImportanceSampler(const SceneConfig &config, int min_samples = 4, int max_samples = 64)
        : config(config), min_samples(min_samples), max_samples(max_samples) {}
    /**
     * @struct SampleStats
     * @brief A structure to hold statistics about the samples.
     */
    struct SampleStats
    {
        Vec3 mean;
        Vec3 variance;
        int samples;
        float importance;
    };

    float calculate_luminance(const Vec3 &color) const;
    float calculate_importance(const Vec3 &pixel_color) const;

private:
    const SceneConfig &config;
    int min_samples;
    int max_samples;
};

#endif
