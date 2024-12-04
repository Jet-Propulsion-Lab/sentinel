// BEWARE
// WE ARE NOT SCIENTISTS, WE ARE ENGINEERS
// What you will see in this file will defy all common expectation, sense, and logic
// Please do not touch anything unless you are absolutely sure of what you are doing

#pragma once

#ifndef GRAVITY_STANDARD
    #define GRAVITY_STANDARD 9.80665
#endif

/// ### A three dimensional vector
/// 04/12/2024 | 1.0.0 | No known issues
/// Hamza Bin Aamir
class Vector3{
public:
    double x, y, z;
    Vector3(double x, double y, double z) : x(x), y(y), z(z) {}
    Vector3() : x(0), y(0), z(0) {}

    /// ### Adds a vector to another vector
    /// @param v The vector to be added
    /// @returns The sum of the two vectors
    Vector3 operator+(const Vector3& v) const{
        return Vector3(x + v.x, y + v.y, z + v.z);
    }

    /// ### Subtracts a vector to another vector
    /// @param v The vector to be subtracted
    /// @returns The difference of the two vectors
    Vector3 operator-(const Vector3& v) const{
        return Vector3(x - v.x, y - v.y, z - v.z);
    }

    /// ### Multiplies a vector by a scalar
    /// @param s The scalar to multiply by
    /// @returns The product of the vector and the scalar
    Vector3 operator*(double s) const{
        return Vector3(x * s, y * s, z * s);
    }

    /// ### Divides a vector by a scalar
    /// @param s The scalar to divide by
    /// @returns The quotient of the vector and the scalar
    Vector3 operator/(double s) const{
        return Vector3(x / s, y / s, z / s);
    }

    /// ### Calculates the dot product of two vectors
    /// @param v The vector to calculate the dot product with
    /// @returns The dot product of the two vectors
    double operator*(const Vector3& v) const{
        return x * v.x + y * v.y + z * v.z;
    }

    /// ### Calculates the cross product of two vectors
    /// @param v The vector to calculate the cross product with
    /// @returns The cross product of the two vectors
    Vector3 operator^(const Vector3& v) const{
        return Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }
};