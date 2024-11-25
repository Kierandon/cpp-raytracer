#ifndef VEC3_H
#define VEC3_H

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

/**
 * @class Vec3
 * @brief Represents a 3D vector with floating-point components.
 *        This class provides various vector operations commonly used in 3D graphics, such as vector addition,
 *        subtraction, dot and cross products, normalization, and reflection/refractive calculations.
 */
class Vec3
{
public:
    float x, y, z; ///< The components of the vector (x, y, z).

    // Constructors
    /**
     * @brief Default constructor, initializes the vector to (0, 0, 0).
     */
    Vec3();

    /**
     * @brief Initializes the vector with the same value for all components.
     * @param v The value for all components (x, y, z).
     */
    Vec3(float v);

    /**
     * @brief Initializes the vector with specific values for each component.
     * @param x The x-component of the vector.
     * @param y The y-component of the vector.
     * @param z The z-component of the vector.
     */
    Vec3(float x, float y, float z);

    // Operators
    /**
     * @brief Negates the vector (i.e., multiplies all components by -1).
     * @return A new vector with the negated components.
     */
    Vec3 operator-() const;

    /**
     * @brief Adds two vectors together.
     * @param v The vector to add.
     * @return A new vector that is the sum of the two vectors.
     */
    Vec3 operator+(const Vec3 &v) const;

    /**
     * @brief Adds another vector to the current vector.
     * @param v The vector to add.
     * @return The current vector after addition.
     */
    Vec3 &operator+=(const Vec3 &v);

    /**
     * @brief Subtracts one vector from another.
     * @param v The vector to subtract.
     * @return A new vector that is the difference of the two vectors.
     */
    Vec3 operator-(const Vec3 &v) const;

    /**
     * @brief Multiplies the vector by a scalar value.
     * @param t The scalar to multiply with.
     * @return A new vector that is the result of the scalar multiplication.
     */
    Vec3 operator*(float t) const;

    /**
     * @brief Multiplies the vector element-wise with another vector.
     * @param v The vector to multiply with.
     * @return A new vector that is the element-wise multiplication of the two vectors.
     */
    Vec3 operator*(const Vec3 &v) const;

    /**
     * @brief Divides the vector by a scalar value.
     * @param t The scalar to divide by.
     * @return A new vector that is the result of the division.
     */
    Vec3 operator/(float t) const;

    /**
     * @brief Divides the vector element-wise by another vector.
     * @param v The vector to divide by.
     * @return A new vector that is the element-wise division of the two vectors.
     */
    Vec3 operator/(const Vec3 &v) const;

    /**
     * @brief Divides the vector by a scalar value and updates the current vector.
     * @param t The scalar to divide by.
     * @return The current vector after division.
     */
    Vec3 &operator/=(const float t);

    // Utility functions
    /**
     * @brief Calculates the length (magnitude) of the vector.
     * @return The length of the vector.
     */
    float length() const;

    /**
     * @brief Normalizes the vector (scales the vector to have a length of 1).
     * @return A new vector with the same direction but a length of 1.
     */
    Vec3 normalized() const;

    // Dot and Cross Product
    /**
     * @brief Calculates the dot product of this vector and another vector.
     * @param v The other vector.
     * @return The dot product of the two vectors.
     */
    float dot(const Vec3 &v) const;

    /**
     * @brief Calculates the cross product of this vector and another vector.
     * @param v The other vector.
     * @return A new vector that is the cross product of the two vectors.
     */
    Vec3 cross(const Vec3 &v) const;

    // Friend functions for output
    /**
     * @brief Outputs the vector to an output stream.
     * @param out The output stream.
     * @param v The vector to output.
     * @return The output stream with the vector data.
     */
    friend std::ostream &operator<<(std::ostream &out, const Vec3 &v);

    /**
     * @brief Multiplies a scalar by a vector.
     * @param t The scalar value.
     * @param v The vector to multiply.
     * @return A new vector that is the result of the multiplication.
     */
    friend Vec3 operator*(float t, const Vec3 &v);

    // The below need to be put in the cpp file
    /**
     * @brief Reflects a vector off a surface defined by a normal vector.
     * @param v The incoming vector (e.g., a ray).
     * @param n The surface normal vector.
     * @return The reflected vector.
     */
    Vec3 reflect(const Vec3 &v, const Vec3 &n) const;

    /**
     * @brief Refracts a vector based on the surface normal and the index of refraction.
     * @param v The incoming vector (e.g., a ray).
     * @param n The surface normal vector.
     * @param eta The ratio of the indices of refraction.
     * @return The refracted vector.
     */
    Vec3 refract(const Vec3 &v, const Vec3 &n, float eta) const;

    /**
     * @brief Returns the component-wise minimum of this vector and another vector.
     * @param other The other vector.
     * @return A new vector containing the minimum values between the two vectors' components.
     */
    Vec3 min(const Vec3 &other) const;

    /**
     * @brief Returns the component-wise maximum of this vector and another vector.
     * @param other The other vector.
     * @return A new vector containing the maximum values between the two vectors' components.
     */
    Vec3 max(const Vec3 &other) const;

    /**
     * @brief Accesses the component at a specific index (0 for x, 1 for y, 2 for z).
     * @param i The index (0, 1, or 2).
     * @return The component at the specified index.
     */
    float operator[](int i) const;

    /**
     * @brief Checks if the vector is near zero (close to the origin).
     * @return True if the vector is near zero, false otherwise.
     */
    bool near_zero() const;
};

#endif // Vec3_H
