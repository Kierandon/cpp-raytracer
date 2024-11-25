#include "core/ImportanceSampler.h"

float ImportanceSampler::calculate_luminance(const Vec3& color) const {
        return 0.299f * color.x + 0.587f * color.y + 0.114f * color.z;
}

float ImportanceSampler::calculate_importance(const Vec3& pixel_color) const {
    float lum = calculate_luminance(pixel_color);
    return lum * (1.0f - lum);
}
