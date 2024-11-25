#ifndef CAMERA_H
#define CAMERA_H

#include "core/Ray.h"
#include "core/Vec3.h"

class Camera
{
public:
    /**
     * @brief Constructs a new Camera object.
     *
     * @param lookfrom The position of the camera in world space.
     * @param lookat The point in world space that the camera is looking at.
     * @param vup The "up" direction in world space.
     * @param vfov The vertical field of view in degrees.
     * @param aspect_ratio The aspect ratio of the viewport (width/height).
     * @param aperture The aperture size for depth of field effects.
     * @param focus_dist The distance from the camera to the focus plane.
     * @param enable_dof A boolean flag to enable or disable depth of field.
     */
    Camera(Vec3 lookfrom, Vec3 lookat, Vec3 vup,
           float vfov, float aspect_ratio,
           float aperture, float focus_dist,
           bool enable_dof);

    /**
     * @brief Returns a ray from the camera through the viewport at the given
     *       coordinates.
     */
    Ray get_ray(float s, float t) const;

private:
    Vec3 origin;
    Vec3 lower_left;
    Vec3 horizontal;
    Vec3 vertical;
    Vec3 u, v, w;      // Camera basis vectors
    float lens_radius; // Aperture / 2
    bool use_dof;

    Vec3 random_in_unit_disk() const;
};

#endif // CAMERA_H
