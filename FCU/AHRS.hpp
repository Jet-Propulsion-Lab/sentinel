#pragma once

#include "Physics.hpp"

#ifndef AHRS_DEAD_RECKON_STEP
    #define AHRS_DEAD_RECKON_STEP 0.01
#endif

/// ### Singleton class for the Attitude Heading Reference System
/// 04/12/2024 | 1.0.0 | No known issues
/// Hamza Bin Aamir, Syed Areeb Zaheer, and Azeem Liaqat
/// The Attitude Heading Reference System contains information about the current orientation and position of the rocket relative to HOME
class AHRS{
private:
    // Static pointer to the Singleton instance
    static AHRS* AttitudeHeadingReferenceSystem;

    bool Lock = false; // We will make a MUTUAL EXCLUSION LOCK (MUTEX) to lock the AHRS from being written to by multiple threads

    Vector3 Position = Vector3(0, 0, 0); // Initializes to ZERO because we always start at HOME
    Vector3 Rotation = Vector3(0, 0, 0); // Initializes to ZERO because we always start with no rotation
    Vector3 Velocity = Vector3(0, 0, 0); // Initializes to ZERO because we always start with no velocity
    Vector3 Acceleration = Vector3(0, 0, 0); // Initializes to ZERO because we always start with no acceleration
    Vector3 AngularVelocity = Vector3(0, 0, 0); // Initializes to ZERO because we always start with no angular velocity
    Vector3 AngularAcceleration = Vector3(0, 0, 0); // Initializes to ZERO because we always start with no angular acceleration

    // Private Constructor to prevent creation of our singleton
    AHRS() {}

public:
    // ### Specific access is needed to *write* to the AHRS
    // @returns true if access is granted (automatically locks AHRS until released), false if access is denied
    bool RequestAccess() { if (!Lock) { Lock = true; return true; } else return false; };
    // There is no specific enforcement of the release of the AHRS, therefore, any thread, including ones currently not locking the AHRS, can release it
    // However, such a condition will likely cause SIGNIFICANT PAIN AND SUFFERING, and therefore is highly discouraged.
    void ReleaseAccess() { Lock = false; };
    
    /// ### Gets the current position of the rocket relative to HOME
    /// @returns X is the East-West position, Y is the North-South position, and Z is the Up-Down position
    Vector3 GetPosition() { return Position; }

    /// ### Gets the current rotation of the rocket relative to HOME
    /// @returns X is the pitch, Y is the yaw, and Z is the roll
    Vector3 GetRotation() { return Rotation; }

    /// ### Gets the current velocity of the rocket relative to HOME
    /// @returns X is the East-West velocity, Y is the North-South velocity, and Z is the Up-Down velocity
    Vector3 GetVelocity() { return Velocity; }

    /// ### Gets the current acceleration of the rocket relative to HOME
    /// @returns X is the East-West acceleration, Y is the North-South acceleration, and Z is the Up-Down acceleration
    Vector3 GetAcceleration() { return Acceleration; }

    /// ### Gets the current angular velocity of the rocket relative to HOME
    /// @returns X is the pitch rate, Y is the yaw rate, and Z is the roll rate
    Vector3 GetAngularVelocity() { return AngularVelocity; }

    /// ### Gets the current angular acceleration of the rocket relative to HOME
    /// @returns X is the pitch acceleration, Y is the yaw acceleration, and Z is the roll acceleration
    Vector3 GetAngularAcceleration() { return AngularAcceleration; }
};