#ifndef UTILS_H
#define UTILS_H

#include "core/Vec3.h"

/**
 * Generate a random float between 0 and 1 using mt19937 generator.
 * @return Random float in [0,1]
 */
float random_float();

/**
 * Generate a random float between min and max.
 * @param min Minimum value
 * @param max Maximum value
 * @return Random float in [min,max]
 */
float random_float(float min, float max);

/**
 * Calculate reflection vector around a normal.
 * @param v Incident vector
 * @param n Surface normal
 * @return Reflected vector
 */
Vec3 reflect(const Vec3 &v, const Vec3 &n);

/**
 * Calculate refraction vector using Snell's law.
 * @param v Incident vector
 * @param n Surface normal
 * @param eta Ratio of refractive indices (n1/n2)
 * @return Refracted vector, or zero vector if total internal reflection occurs
 */
Vec3 refract(const Vec3 &v, const Vec3 &n, float eta);

/**
 * Calculate Schlick approximation for Fresnel equations.
 * @param cosine Cosine of angle between normal and view direction
 * @param ref_idx Ratio of refractive indices
 * @return Reflection coefficient
 */
float schlick(float cosine, float ref_idx);

/**
 * Generate random point inside unit sphere using rejection sampling.
 * @return Random point inside unit sphere
 */
Vec3 random_point_in_unit_sphere();

/**
 * Generate random direction vector weighted by cosine of angle from normal.
 * Uses concentric disk mapping for better sampling.
 * @param normal Surface normal to generate direction around
 * @return Normalized random direction vector
 */
Vec3 random_cosine_direction(const Vec3 &normal);

/**
 * Generate random unit vector using spherical coordinates.
 * @return Random normalized direction vector
 */
Vec3 random_unit_vector();

#endif // UTILS_H
