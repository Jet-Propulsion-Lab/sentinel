//HAL

#include <inttypes.h>
#include <chrono>

struct Force{
    float Newtons;
    float kGf;
};

class StrainSensor{
public:
    float Data; // Strain is unitless
    std::chrono::milliseconds LastUpdateTime;
    /// @brief DO NOT USE THIS AS AN IDENTIFIER, THIS IS USED TO SET THE INSTANCE ID, USE InstanceID INSTEAD
    static int InstanceCount;
    int InstanceID;
};
int StrainSensor::InstanceCount = 0;

class ForceSensor{
public:
    Force Data;
    std::chrono::milliseconds LastUpdateTime;
    /// @brief DO NOT USE THIS AS AN IDENTIFIER, THIS IS USED TO SET THE INSTANCE ID, USE InstanceID INSTEAD
    static int InstanceCount;
    int InstanceID;
};
int ForceSensor::InstanceCount = 0;