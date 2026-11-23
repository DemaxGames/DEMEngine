#pragma once
#include "core/math/vec.h"

namespace dem{

class Rigidbody{
public:
    math::vec3 velocity;
    math::vec3 acceleration;
    math::vec3 forceAccumulator;
    float mass;
    float inverseMass;
    float drag;
    bool isKinematic;
    bool useGravity;

    Rigidbody();
    void AddForce(const math::vec3& force);
    void ClearForces();
    void Integrate(float deltaTime);
};

}
