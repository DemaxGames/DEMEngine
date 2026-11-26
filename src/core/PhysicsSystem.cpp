#include "core/PhysicsSystem.h"
#include "core/Logger/Logger.h"

namespace dem{

PhysicsSystem::PhysicsSystem(){
    earthTransform = nullptr;
    moonTransform = nullptr;

    // Gravitational parameters scaled for 1 unit = 100m
    // Earth: g = 9.8 m/s^2 = 0.098 units/s^2 at r = 6000 units
    // GM = g * r^2 = 0.098 * 6000^2 = 3,528,000
    earthGM = 35280000.0f;

    // Moon: g = 1.62 m/s^2 = 0.0162 units/s^2 at r = 2000 units
    // GM = g * r^2 = 0.0162 * 2000^2 = 64,800
    moonGM = 648000.0f;

    // Body radii
    earthRadius = 6000.0f;
    moonRadius = 2000.0f;

    // Moon SoI: a * (m/M)^(2/5)
    // a = 380000, m/M = 64800/3528000 = 0.01837
    // (0.01837)^0.4 ≈ 0.199
    // SoI ≈ 380000 * 0.199 ≈ 75620
    moonSoI = 75620.0f;

    // Atmosphere: top at 70 km = 700 units above surface
    atmosphereTop = 700.0f;
    atmosphereScaleHeight = 20.0f;  // units
    seaLevelDensity = 100.0f;       // arbitrary units (100 kPa)

    // Drag coefficient (Cd * A combined)
    dragCoefficient = 0.5f;

    // Time warp
    timeWarp = 1.0f;
    warpEnabled = false;
}

math::vec3 PhysicsSystem::GetGravityAcceleration(const math::vec3& position){
    math::vec3 accel(0.f, 0.f, 0.f);

    if(earthTransform == nullptr) return accel;

    math::vec3 earthPos = earthTransform->GetWorldPosition();
    math::vec3 toEarth = earthPos - position;
    float distToEarth = std::sqrt(toEarth.data[0]*toEarth.data[0] + 
                                   toEarth.data[1]*toEarth.data[1] + 
                                   toEarth.data[2]*toEarth.data[2]);

    // Check if inside Moon's SoI
    bool inMoonSoI = false;
    if(moonTransform != nullptr){
        math::vec3 moonPos = moonTransform->GetWorldPosition();
        math::vec3 toMoon = moonPos - position;
        float distToMoon = std::sqrt(toMoon.data[0]*toMoon.data[0] + 
                                       toMoon.data[1]*toMoon.data[1] + 
                                       toMoon.data[2]*toMoon.data[2]);
        if(distToMoon < moonSoI){
            inMoonSoI = true;
        }
    }

    if(inMoonSoI && moonTransform != nullptr){
        // Moon gravity only
        math::vec3 moonPos = moonTransform->GetWorldPosition();
        math::vec3 toMoon = moonPos - position;
        float distToMoon = std::sqrt(toMoon.data[0]*toMoon.data[0] + 
                                       toMoon.data[1]*toMoon.data[1] + 
                                       toMoon.data[2]*toMoon.data[2]);
        if(distToMoon > 0.0f){
            float invDistCube = 1.0f / (distToMoon * distToMoon * distToMoon);
            accel = toMoon * (moonGM * invDistCube);
        }
    } else {
        // Earth gravity only
        if(distToEarth > 0.0f){
            float invDistCube = 1.0f / (distToEarth * distToEarth * distToEarth);
            accel = toEarth * (earthGM * invDistCube);
        }
    }

    return accel;
}

float PhysicsSystem::GetAltitudeAboveEarth(const math::vec3& position){
    if(earthTransform == nullptr) return 0.0f;
    math::vec3 earthPos = earthTransform->GetWorldPosition();
    math::vec3 pos = position;  // copy because vec3::operator- is not const
    math::vec3 toEarth = pos - earthPos;
    float dist = std::sqrt(toEarth.data[0]*toEarth.data[0] + 
                             toEarth.data[1]*toEarth.data[1] + 
                             toEarth.data[2]*toEarth.data[2]);
    return dist - earthRadius;
}

float PhysicsSystem::GetDensity(float altitude){
    if(altitude < 0.0f) altitude = 0.0f;
    if(altitude > atmosphereTop) return 0.0f;
    return seaLevelDensity * (float)std::exp(-altitude / atmosphereScaleHeight);
}

void PhysicsSystem::ApplyGravity(Rigidbody* rb, TransformComponent* transform, float dt){
    if(!rb->useGravity || transform == nullptr) return;

    math::vec3 gravity = GetGravityAcceleration(transform->position);
    rb->AddForce(gravity * rb->mass);  // F = m * g
}

void PhysicsSystem::ApplyAtmosphericDrag(Rigidbody* rb, TransformComponent* transform, float dt){
    if(earthTransform == nullptr || transform == nullptr) return;

    float altitude = GetAltitudeAboveEarth(transform->position);
    if(altitude > atmosphereTop) return;  // Outside atmosphere

    float density = GetDensity(altitude);
    if(density <= 0.0f) return;

    // Drag force: F_d = -0.5 * rho * v^2 * Cd * A * v_hat
    // Simplified: F_d = -dragCoeff * density * v * |v|
    float speed = std::sqrt(rb->velocity.data[0]*rb->velocity.data[0] + 
                              rb->velocity.data[1]*rb->velocity.data[1] + 
                              rb->velocity.data[2]*rb->velocity.data[2]);

    if(speed > 0.0f){
        float dragMag = 0.5f * density * speed * speed * dragCoefficient;
        math::vec3 dragForce = rb->velocity * (-dragMag / speed);  // -v_hat * dragMag
        rb->AddForce(dragForce);
    }
}

void PhysicsSystem::Update(float deltaTime){
    if(earthTransform == nullptr) return;

    float total = deltaTime * timeWarp;
    if(total <= 0.0f) return;

    // Substep so large time-warp multipliers remain stable
    const float maxStep = 0.02f;
    int steps = (int)std::ceil(total / maxStep);
    if(steps < 1) steps = 1;
    if(steps > 5000) steps = 5000;
    float dt = total / (float)steps;

    for(int i = 0; i < ecs::entities.size(); i++){
        Rigidbody* rb = ecs::entities[i].GetComponent<Rigidbody>();
        TransformComponent* transform = ecs::entities[i].GetComponent<TransformComponent>();

        if(rb == nullptr || transform == nullptr) continue;
        if(rb->isKinematic) continue;

        // Preserve externally applied forces (e.g. thrust) for every substep
        math::vec3 externalForce = rb->forceAccumulator;

        for(int s = 0; s < steps; s++){
            rb->forceAccumulator = externalForce;

            // Apply gravity (always, even during warp - orbits propagate)
            ApplyGravity(rb, transform, dt);

            // Apply atmospheric drag (even during warp)
            ApplyAtmosphericDrag(rb, transform, dt);

            // Integrate velocity and advance position
            rb->Integrate(dt);
            transform->position = transform->position + rb->velocity * dt;
        }
    }
}

}
