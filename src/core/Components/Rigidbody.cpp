#include "core/Components/Rigidbody.h"

#include <iostream>

namespace dem{

Rigidbody::Rigidbody(){
    velocity = math::vec3(0.f, 0.f, 0.f);
    acceleration = math::vec3(0.f, 0.f, 0.f);
    forceAccumulator = math::vec3(0.f, 0.f, 0.f);
    mass = 1.0f;
    inverseMass = 1.0f / mass;
    drag = 0.0f;
    isKinematic = false;
    useGravity = true;
}

void Rigidbody::AddForce(const math::vec3& force){
    forceAccumulator = forceAccumulator + force;
}

void Rigidbody::ClearForces(){
    forceAccumulator = math::vec3(0.f, 0.f, 0.f);
}

void Rigidbody::Integrate(float deltaTime){
    if(isKinematic) return;

    // F = ma => a = F/m
    acceleration = forceAccumulator * inverseMass;

    
    // v = v + a * dt
    velocity = velocity + acceleration * deltaTime;

    // Apply damping/drag
    if(drag > 0.0f){
        float damping = 1.0f / (1.0f + drag * deltaTime);
        velocity = velocity * damping;
    }

    // Clear forces for next frame
    forceAccumulator = math::vec3(0.f, 0.f, 0.f);
}

}
