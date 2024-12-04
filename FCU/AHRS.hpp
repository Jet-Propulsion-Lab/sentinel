#pragma once

#include "Physics.hpp"

#ifndef AHRS_DEAD_RECKON_STEP
    // How much time (in seconds) should dead reckoning wait before kicking in
    #define AHRS_DEAD_RECKON_STEP 0.1
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

    static Vector3 Position; // Initializes to ZERO because we always start at HOME
    static Vector3 Rotation; // Initializes to ZERO because we always start with no rotation
    static Vector3 Velocity; // Initializes to ZERO because we always start with no velocity
    static Vector3 Acceleration; // Initializes to ZERO because we always start with no acceleration
    static Vector3 AngularVelocity; // Initializes to ZERO because we always start with no angular velocity
    static Vector3 AngularAcceleration; // Initializes to ZERO because we always start with no angular acceleration

    static double LastPositionReckon;
    static double LastRotationReckon;
    static double LastVelocityReckon;
    static double LastAngVelocReckon;

    // Private Constructor to prevent creation of our singleton
    AHRS() {}

public:
    // Deleting the copy constructor to prevent copies
    AHRS(const AHRS& obj) = delete;

    // Static method to get our Singleton AHRS
    static AHRS* getInstance() {
        if (AttitudeHeadingReferenceSystem == nullptr) {
            if (AttitudeHeadingReferenceSystem == nullptr) {
                AttitudeHeadingReferenceSystem = new AHRS();
            }
        }
        return AttitudeHeadingReferenceSystem;
    }

    // ### Specific access is needed to *write* to the AHRS
    // @returns true if access is granted (automatically locks AHRS until released), false if access is denied
    bool RequestAccess() { if (!Lock) { Lock = true; return true; } else return false; };
    // There is no specific enforcement of the release of the AHRS, therefore, any thread, including ones currently not locking the AHRS, can release it
    // However, such a condition will likely cause SIGNIFICANT PAIN AND SUFFERING, and therefore is highly discouraged.
    void ReleaseAccess() { Lock = false; };
    
    /// ### Gets the current position of the rocket relative to HOME
    /// @returns X is the East-West position, Y is the North-South position, and Z is the Up-Down position
    const Vector3& GetPosition() { return Position; }

    /// ### Gets the current rotation of the rocket relative to HOME
    /// @returns X is the pitch, Y is the yaw, and Z is the roll
    const Vector3& GetRotation() { return Rotation; }

    /// ### Gets the current velocity of the rocket relative to HOME
    /// @returns X is the East-West velocity, Y is the North-South velocity, and Z is the Up-Down velocity
    const Vector3& GetVelocity() { return Velocity; }

    /// ### Gets the current acceleration of the rocket relative to HOME
    /// @returns X is the East-West acceleration, Y is the North-South acceleration, and Z is the Up-Down acceleration
    const Vector3& GetAcceleration() { return Acceleration; }

    /// ### Gets the current angular velocity of the rocket relative to HOME
    /// @returns X is the pitch rate, Y is the yaw rate, and Z is the roll rate
    const Vector3& GetAngularVelocity() { return AngularVelocity; }

    /// ### Gets the current angular acceleration of the rocket relative to HOME
    /// @returns X is the pitch acceleration, Y is the yaw acceleration, and Z is the roll acceleration
    const Vector3& GetAngularAcceleration() { return AngularAcceleration; }

    /// ### Sets the current position of the rocket relative to HOME
    /// @param TIME_STANDARD The current time in seconds
    void UpdateDeadReckon(const double& TIME_STANDARD){
        if(TIME_STANDARD-LastRotationReckon > AHRS_DEAD_RECKON_STEP){
            Rotation = Rotation + (AngularVelocity * (TIME_STANDARD-LastRotationReckon));
            LastAngVelocReckon = TIME_STANDARD;
        }
        if(TIME_STANDARD-LastAngVelocReckon > AHRS_DEAD_RECKON_STEP){
            AngularVelocity = AngularVelocity + (AngularAcceleration * (TIME_STANDARD-LastAngVelocReckon));
            LastVelocityReckon = TIME_STANDARD;
        }
        if(TIME_STANDARD-LastVelocityReckon > AHRS_DEAD_RECKON_STEP){
            Velocity = Velocity + (Acceleration * (TIME_STANDARD-LastVelocityReckon));
            LastVelocityReckon = TIME_STANDARD;
        }
        if(TIME_STANDARD-LastPositionReckon > AHRS_DEAD_RECKON_STEP){
            Position = Position + (Velocity * (TIME_STANDARD-LastPositionReckon));
            LastPositionReckon = TIME_STANDARD;
        }
    }
};

Vector3 AHRS::Position = Vector3(0, 0, 0); // Initializes to ZERO because we always start at HOME
Vector3 AHRS::Rotation = Vector3(0, 0, 0); // Initializes to ZERO because we always start with no rotation
Vector3 AHRS::Velocity = Vector3(0, 0, 0); // Initializes to ZERO because we always start with no velocity
Vector3 AHRS::Acceleration = Vector3(0, 0, 0); // Initializes to ZERO because we always start with no acceleration
Vector3 AHRS::AngularVelocity = Vector3(0, 0, 0); // Initializes to ZERO because we always start with no angular velocity
Vector3 AHRS::AngularAcceleration = Vector3(0, 0, 0); // Initializes to ZERO because we always start with no angular acceleration

double AHRS::LastPositionReckon = 0;
double AHRS::LastRotationReckon = 0;
double AHRS::LastVelocityReckon = 0;
double AHRS::LastAngVelocReckon = 0;