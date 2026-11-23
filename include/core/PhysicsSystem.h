#pragma once

#include "core/Components/TransformComponent.h"
#include "core/Components/Rigidbody.h"
#include "core/ECS/ecs.h"
#include "core/math/vec.h"
#include <cmath>

namespace dem{

class PhysicsSystem{
public:
    // Celestial body transforms
    TransformComponent* earthTransform;
    TransformComponent* moonTransform;

    // Gravitational parameters (units^3 / s^2)
    float earthGM;
    float moonGM;

    // Sphere of influence radii (units)
    float moonSoI;

    // Body radii (units)
    float earthRadius;
    float moonRadius;

    // Atmosphere
    float atmosphereTop;        // altitude above earth surface where atmosphere ends (units)
    float atmosphereScaleHeight; // scale height for exponential density decay (units)
    float seaLevelDensity;      // density at surface (arbitrary units)

    // Drag
    float dragCoefficient;      // Cd * A combined factor

    // Time warp
    float timeWarp;             // 1, 2, 10, 100, 1000
    bool warpEnabled;

    PhysicsSystem();
    void Update(float deltaTime);

    float GetAltitudeAboveEarth(const math::vec3& position);

private:
    void ApplyGravity(Rigidbody* rb, TransformComponent* transform, float dt);
    void ApplyAtmosphericDrag(Rigidbody* rb, TransformComponent* transform, float dt);
    math::vec3 GetGravityAcceleration(const math::vec3& position);
    float GetDensity(float altitude);
};

}
