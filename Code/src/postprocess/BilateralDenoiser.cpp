#include "postprocess/BilateralDenoiser.h"

#include <omp.h>
#include <mutex>
#include <iostream>
#include <iomanip>
#include <atomic>

void BilateralDenoiser::denoise()
{
    int width = config.image_width;
    int height = config.image_height;
    std::vector<Vec3> output(pixels.size());

    int radius = static_cast<int>(2 * sigmaSpatial);
    float twoSigmaSpatialSq = 2 * sigmaSpatial * sigmaSpatial;
    float twoSigmaRangeSq = 2 * sigmaRange * sigmaRange;

    // Progress tracking
    std::atomic<int> progress{0};
    const int totalPixels = width * height;
    const int progressStep = totalPixels / 100; // 5% steps
    std::mutex printMutex;

#pragma omp parallel for schedule(dynamic) collapse(2)
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            Vec3 sum(0.0f);
            float weightSum = 0.0f;
            Vec3 centerColor = pixels[y * width + x];

            for (int ky = -radius; ky <= radius; ++ky)
            {
                int ny = y + ky;
                if (ny < 0 || ny >= height)
                    continue;

                for (int kx = -radius; kx <= radius; ++kx)
                {
                    int nx = x + kx;
                    if (nx < 0 || nx >= width)
                        continue;

                    Vec3 neighborColor = pixels[ny * width + nx];
                    float spatialDistSq = kx * kx + ky * ky;
                    float colorDistSq = (neighborColor - centerColor).dot(neighborColor - centerColor);

                    float weight = std::exp(-spatialDistSq / twoSigmaSpatialSq - colorDistSq / twoSigmaRangeSq);
                    sum += neighborColor * weight;
                    weightSum += weight;
                }
            }

            output[y * width + x] = sum / weightSum;

            int currentProgress = ++progress;
            if (currentProgress % progressStep == 0)
            {
                std::lock_guard<std::mutex> lock(printMutex);
                std::cout << "\rDenoising progress: "
                          << std::fixed << std::setprecision(1)
                          << (currentProgress * 100.0f / totalPixels) << "%"
                          << std::flush;
            }
        }
    }

    std::cout << "\rDenoising progress: 100.0%" << std::endl;
    pixels = std::move(output);
}
