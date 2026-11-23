#include <iostream>
#include <stdio.h>
#include <filesystem>
#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>
#include "core/dem.h"
#include "core/time/time.h"

#define PI 3.141592f

using namespace dem;

// ---------- small vector helpers (avoid const-incompatible engine operators) ----------
static math::vec3 vmake(float x, float y, float z){ return math::vec3(x, y, z); }
static math::vec3 vsub(const math::vec3& a, const math::vec3& b){
    return math::vec3(a.data[0]-b.data[0], a.data[1]-b.data[1], a.data[2]-b.data[2]);
}
static math::vec3 vadd(const math::vec3& a, const math::vec3& b){
    return math::vec3(a.data[0]+b.data[0], a.data[1]+b.data[1], a.data[2]+b.data[2]);
}
static math::vec3 vscale(const math::vec3& a, float s){
    return math::vec3(a.data[0]*s, a.data[1]*s, a.data[2]*s);
}
static float vdot(const math::vec3& a, const math::vec3& b){
    return a.data[0]*b.data[0] + a.data[1]*b.data[1] + a.data[2]*b.data[2];
}
static float vlen(const math::vec3& a){
    return std::sqrt(a.data[0]*a.data[0] + a.data[1]*a.data[1] + a.data[2]*a.data[2]);
}
static math::vec3 vcross(const math::vec3& a, const math::vec3& b){
    return math::vec3(
        a.data[1]*b.data[2] - a.data[2]*b.data[1],
        a.data[2]*b.data[0] - a.data[0]*b.data[2],
        a.data[0]*b.data[1] - a.data[1]*b.data[0]);
}

// ---------- globals ----------
dem::ecs::Entity earthEntity, moonEntity, csmEntity, cameraEntity, lightEntity, hudEntity;

dem::TransformComponent* earthTransform = nullptr;
dem::TransformComponent* moonTransform = nullptr;
dem::TransformComponent* csmTransform = nullptr;
dem::TransformComponent* cameraTransform = nullptr;
dem::TransformComponent* hudTransform = nullptr;
dem::CameraComponent* cameraComponent = nullptr;
dem::Rigidbody* csmRigidbody = nullptr;
dem::TextRenderer* hudText = nullptr;

dem::PhysicsSystem physics;

// meshes / textures
dem::Mesh sphereMesh, csmMesh;
dem::Image earthTexture, moonTexture, csmTexture;
dem::Material* earthMat = nullptr;
dem::Material* moonMat = nullptr;
dem::Material* csmMat = nullptr;
dem::CharSet font;

// game state
float thrust = 0.0f;
const float maxThrust = 200.0f;
const float thrustStep = 80.0f;
float timeWarp = 1.0f;
bool showOrbitInfo = false;
bool gameOver = false;
bool landed = false;

// camera
float cameraDistance = 120.0f;
float cameraYaw = PI * 0.5f;
float cameraPitch = 0.2f;

// constants kept in sync with PhysicsSystem
const float t_earthRadius = 6000.0f;
const float t_moonRadius = 2000.0f;
const float t_moonDistance = 380000.0f;

// death screen
bool deathStarted = false;
float deathTimer = 0.0f;
int deathCharIndex = 0;
std::string deathMessage = "MISSION FAILED";
float hintTimer = 0.0f;

// ---------- orbit math ----------
struct OrbitData{
    float altitude;
    float apoAlt;
    float periAlt;
    float period;
    bool escape;
    bool suborbital;
    bool valid;
};

static OrbitData ComputeOrbit(const math::vec3& relPos, const math::vec3& relVel, float mu, float bodyRadius){
    OrbitData o;
    o.altitude = vlen(relPos) - bodyRadius;
    o.apoAlt = 0; o.periAlt = 0; o.period = 0;
    o.escape = false; o.suborbital = false; o.valid = false;

    float r = vlen(relPos);
    if(r <= 0.0001f) return o;
    o.valid = true;

    float v2 = vdot(relVel, relVel);
    float eps = 0.5f * v2 - mu / r;
    math::vec3 h = vcross(relPos, relVel);
    float hmag = vlen(h);
    float ecc2 = 1.0f + 2.0f * eps * hmag * hmag / (mu * mu);
    if(ecc2 < 0.0f) ecc2 = 0.0f;
    float ecc = std::sqrt(ecc2);

    if(eps < 0.0f && ecc < 1.0f){
        float a = -mu / (2.0f * eps);
        float apoR = a * (1.0f + ecc);
        float periR = a * (1.0f - ecc);
        o.apoAlt = apoR - bodyRadius;
        o.periAlt = periR - bodyRadius;
        o.period = 2.0f * PI * std::sqrt((a*a*a) / mu);
        o.suborbital = (periR < bodyRadius);
    } else {
        o.escape = true;
        o.apoAlt = 1e9f;
        o.periAlt = o.altitude;
    }
    return o;
}

static float EstimateBurnTime(float currentRadius, float targetApoRadius, float currentSpeed, float mu, float mass){
    if(thrust <= 0.001f) return -1.0f;
    float aTarget = 0.5f * (currentRadius + targetApoRadius);
    float vNeeded = std::sqrt(mu * (2.0f / currentRadius - 1.0f / aTarget));
    float dv = vNeeded - currentSpeed;
    if(dv <= 0.0f) return 0.0f;
    // burnTime = dv * mass / thrustForce
    return dv * mass / thrust;
}

// ---------- input ----------
void UpdateInput(float deltaTime){
    // thrust throttle
    if(dem::Input::GetKey(dem::KeyCode::LShift) || dem::Input::GetKey(dem::KeyCode::RShift)){
        thrust += thrustStep * deltaTime;
        if(thrust > maxThrust) thrust = maxThrust;
    }
    if(dem::Input::GetKey(dem::KeyCode::LCtrl) || dem::Input::GetKey(dem::KeyCode::RCtrl)){
        thrust -= thrustStep * deltaTime;
        if(thrust < 0.0f) thrust = 0.0f;
    }
    if(dem::Input::GetKeyDown(dem::KeyCode::X)) thrust = 0.0f;

    // update orientation vectors
    csmTransform->GetModelMatrix();

    // apply thrust along vessel forward (only when not warping and not landed)
    if(thrust > 0.0f && timeWarp <= 1.0f && !landed){
        csmRigidbody->AddForce(vscale(csmTransform->forward, thrust));
    }

    // rotation (only when not warping)
    if(timeWarp <= 1.0f && !landed){
        float rot = 1.0f * deltaTime;
        if(dem::Input::GetKey(dem::KeyCode::W)) csmTransform->rotation.x += rot;
        if(dem::Input::GetKey(dem::KeyCode::S)) csmTransform->rotation.x -= rot;
        if(dem::Input::GetKey(dem::KeyCode::A)) csmTransform->rotation.y -= rot;
        if(dem::Input::GetKey(dem::KeyCode::D)) csmTransform->rotation.y += rot;
        if(dem::Input::GetKey(dem::KeyCode::Q)) csmTransform->rotation.z -= rot;
        if(dem::Input::GetKey(dem::KeyCode::E)) csmTransform->rotation.z += rot;
    }

    // time warp cycle
    if(dem::Input::GetKeyDown(dem::KeyCode::K)){
        if(timeWarp < 1.5f) timeWarp = 2.0f;
        else if(timeWarp < 5.0f) timeWarp = 10.0f;
        else if(timeWarp < 50.0f) timeWarp = 100.0f;
        else timeWarp = 1000.0f;
    }
    if(dem::Input::GetKeyDown(dem::KeyCode::L)){
        if(timeWarp > 500.0f) timeWarp = 100.0f;
        else if(timeWarp > 50.0f) timeWarp = 10.0f;
        else if(timeWarp > 5.0f) timeWarp = 2.0f;
        else timeWarp = 1.0f;
    }

    // auto-disable warp in atmosphere or inside Moon SoI
    if(timeWarp > 1.0f){
        float alt = physics.GetAltitudeAboveEarth(csmTransform->position);
        float distMoon = vlen(vsub(moonTransform->position, csmTransform->position));
        if(alt < physics.atmosphereTop || distMoon < physics.moonSoI){
            timeWarp = 1.0f;
        }
    }

    // orbit info toggle
    if(dem::Input::GetKeyDown(dem::KeyCode::H)){
        showOrbitInfo = !showOrbitInfo;
    }

    // camera zoom
    float wheel = dem::Input::GetAxisWheel();
    if(wheel < 0.0f) cameraDistance *= 1.1f;
    if(wheel > 0.0f) cameraDistance *= 0.9f;
    if(cameraDistance < 15.0f) cameraDistance = 15.0f;
    if(cameraDistance > 3000.0f) cameraDistance = 3000.0f;

    // camera orbit via mouse
    cameraYaw += dem::Input::GetAxisX() * 0.06f;
    cameraPitch += dem::Input::GetAxisY() * 0.06f;
    if(cameraPitch > 1.45f) cameraPitch = 1.45f;
    if(cameraPitch < -1.45f) cameraPitch = -1.45f;
}

// ---------- camera ----------
void UpdateCamera(){
    float cp = std::cos(cameraPitch);
    float sp = std::sin(cameraPitch);
    float sy = std::sin(cameraYaw);
    float cy = std::cos(cameraYaw);

    math::vec3 camPos = vadd(csmTransform->position, vscale(vmake(cp*sy, sp, cp*cy), cameraDistance));
    cameraTransform->position = camPos;

    math::vec3 look = vsub(csmTransform->position, camPos);
    float len = vlen(look);
    if(len > 0.0001f) look = vscale(look, 1.0f / len);

    float pitch = std::asin(look.data[1]);
    float yaw = std::atan2(-look.data[0], -look.data[2]);
    cameraTransform->rotation = vmake(pitch, yaw, 0.0f);
}

// ---------- landing ----------
static void TryLand(const math::vec3& bodyPos, float bodyRadius){
    math::vec3 rel = vsub(csmTransform->position, bodyPos);
    float dist = vlen(rel);
    float alt = dist - bodyRadius;
    if(alt > 50.0f) return;
    if(alt < -bodyRadius) return;

    math::vec3 normal = (dist > 0.0001f) ? vscale(rel, 1.0f / dist) : vmake(0,1,0);
    float verticalSpeed = vdot(csmRigidbody->velocity, normal); // negative = descending

    // descending too fast -> crash
    if(verticalSpeed <= -2.0f){
        gameOver = true;
        return;
    }
    // slow enough -> stick to surface (engine acts as landing leg)
    if(verticalSpeed > -2.0f){
        landed = true;
        csmRigidbody->isKinematic = true;
        csmRigidbody->velocity = vmake(0,0,0);
        csmRigidbody->useGravity = false;
        csmTransform->position = vadd(bodyPos, vscale(normal, bodyRadius + 5.0f));
        timeWarp = 1.0f;
    }
}

void CheckLanding(){
    if(gameOver || landed) return;
    TryLand(earthTransform->position, t_earthRadius);
    if(gameOver || landed) return;
    TryLand(moonTransform->position, t_moonRadius);
}

// ---------- HUD ----------
void UpdateHUD(){
    if(gameOver){
        return; // death screen handled in main loop
    }

    std::string text;
    if(showOrbitInfo){
        // Determine current SoI
        math::vec3 relEarth = vsub(csmTransform->position, earthTransform->position);
        float distEarth = vlen(relEarth);
        float distMoon = vlen(vsub(csmTransform->position, moonTransform->position));
        bool inMoon = distMoon < physics.moonSoI;

        OrbitData o;
        std::string soi, state;
        if(inMoon){
            soi = "Moon";
            o = ComputeOrbit(vsub(csmTransform->position, moonTransform->position), csmRigidbody->velocity, physics.moonGM, t_moonRadius);
            if(o.suborbital) state = "Suborbital";
            else if(o.escape) state = "Escape";
            else state = "Orbital";
        } else {
            soi = "Earth";
            o = ComputeOrbit(relEarth, csmRigidbody->velocity, physics.earthGM, t_earthRadius);
            if(o.suborbital) state = "Suborbital";
            else if(o.escape) state = "Escape";
            else state = "Orbital";
        }

        text += "SOI: " + soi + "   STATE: " + state + "\n";
        text += "ALT: " + std::to_string((int)o.altitude) + " u\n";
        if(!o.escape){
            text += "AP: " + std::to_string((int)o.apoAlt) + " u\n";
            text += "PE: " + std::to_string((int)o.periAlt) + " u\n";
            text += "T: " + std::to_string((int)o.period) + " s\n";
        } else {
            text += "AP: inf\nPE: " + std::to_string((int)o.periAlt) + " u\n";
        }
        text += "WARP: " + std::to_string((int)timeWarp) + "x\n";
        text += "THR: " + std::to_string((int)(thrust / maxThrust * 100.0f)) + "%\n";

        // Moon transfer analysis (only meaningful in Earth SoI)
        if(!inMoon){
            float currentSpeed = vlen(csmRigidbody->velocity);
            float moonEdge = t_moonDistance - physics.moonSoI;
            float apoRadius = (o.escape ? 1e9f : o.apoAlt + t_earthRadius);
            if(apoRadius >= moonEdge){
                text += "MOON: AP reaches SoI\n";
            } else {
                float bt = EstimateBurnTime(distEarth, moonEdge, currentSpeed, physics.earthGM, csmRigidbody->mass);
                if(bt < 0.0f) text += "MOON: raise thrust\n";
                else text += "MOON burn ~" + std::to_string((int)bt) + "s\n";
            }
        }
    } else {
        text = "H: orbit info   K/L: warp   Shift/Ctrl: thrust   X: cut\n";
        text += "WASD: pitch/yaw   QE: roll   Wheel: zoom   Mouse: camera";
    }

    hudText->SetText(text);
}

// ---------- main ----------
int main(){
    dem::Logger *logger = dem::Logger::get();

    dem::ecs::init();
    dem::ecs::RegisterComponent<dem::TransformComponent>();
    dem::ecs::RegisterComponent<dem::CameraComponent>();
    dem::ecs::RegisterComponent<dem::MeshRenderer>();
    dem::ecs::RegisterComponent<dem::TextRenderer>();
    dem::ecs::RegisterComponent<dem::LightSourceComponent>();
    dem::ecs::RegisterComponent<dem::Rigidbody>();

    dem::Renderer::Init(1920, 1080);
    dem::Input::Init();
    dem::Input::sensivity = 1.0f;

    font.Load("examples/font.txt");

    sphereMesh.Load("assets/sphere.obj");
    csmMesh.Load("assets/CSM.obj");

    earthTexture.Load("assets/earth.png");
    moonTexture.Load("assets/moon.png");
    csmTexture.Load("assets/csm.png");

    earthMat = new dem::Material();
    moonMat = new dem::Material();
    csmMat = new dem::Material();

    earthMat->glImage.image = &earthTexture;
    earthMat->Load();
    moonMat->glImage.image = &moonTexture;
    moonMat->Load();
    csmMat->glImage.image = &csmTexture;
    csmMat->Load();

    // Earth
    earthEntity = dem::ecs::CreateEntity();
    earthTransform = earthEntity.AddComponent<dem::TransformComponent>();
    earthTransform->position = vmake(0,0,0);
    earthTransform->scale = vmake(t_earthRadius, t_earthRadius, t_earthRadius);
    {
        dem::MeshRenderer* r = earthEntity.AddComponent<dem::MeshRenderer>();
        r->mesh = &sphereMesh; r->material = earthMat; r->Load();
    }

    // Moon
    moonEntity = dem::ecs::CreateEntity();
    moonTransform = moonEntity.AddComponent<dem::TransformComponent>();
    moonTransform->position = vmake(t_moonDistance, 0, 0);
    moonTransform->scale = vmake(t_moonRadius, t_moonRadius, t_moonRadius);
    {
        dem::MeshRenderer* r = moonEntity.AddComponent<dem::MeshRenderer>();
        r->mesh = &sphereMesh; r->material = moonMat; r->Load();
    }

    // CSM: start on the Earth surface on the +X axis, pointing radially outward
    csmEntity = dem::ecs::CreateEntity();
    csmTransform = csmEntity.AddComponent<dem::TransformComponent>();
    csmTransform->position = vmake(t_earthRadius + 100.0f, 0.0f, 0.0f);
    csmTransform->rotation = vmake(0.0f, -PI * 0.5f, 0.0f);
    csmTransform->scale = vmake(2.0f, 2.0f, 2.0f);
    csmRigidbody = csmEntity.AddComponent<dem::Rigidbody>();
    csmRigidbody->mass = 1000.0f;
    csmRigidbody->inverseMass =  1.0f / csmRigidbody->mass;
    csmRigidbody->useGravity = true;
    csmRigidbody->isKinematic = false;
    {
        dem::MeshRenderer* r = csmEntity.AddComponent<dem::MeshRenderer>();
        r->mesh = &csmMesh; r->material = csmMat; r->Load();
    }

    // Camera
    cameraEntity = dem::ecs::CreateEntity();
    cameraTransform = cameraEntity.AddComponent<dem::TransformComponent>();
    cameraTransform->position = vmake(t_earthRadius + 220.0f, 0.0f, 0.0f);
    cameraComponent = cameraEntity.AddComponent<dem::CameraComponent>();
    cameraComponent->pTransform = cameraTransform;
    cameraComponent->fov = PI * 0.5f;
    cameraComponent->nearClip = 1.0f;
    cameraComponent->farClip = 2000000.0f;
    cameraComponent->proj = dem::Perspective;

    // Sun / light
    lightEntity = dem::ecs::CreateEntity();
    {
        dem::TransformComponent* lt = lightEntity.AddComponent<dem::TransformComponent>();
        lt->position = vmake(-5000.f, 3000.f, -5000.f);
        dem::LightSourceComponent* l = lightEntity.AddComponent<dem::LightSourceComponent>();
        l->transform = lt;
        l->color = vmake(1.f, 1.f, 1.f);
        l->strength = 1.0f;
    }

    // HUD text
    hudEntity = dem::ecs::CreateEntity();
    hudTransform = hudEntity.AddComponent<dem::TransformComponent>();
    hudTransform->position = vmake(-0.48f, 15.5f, 0.0f);
    hudText = hudEntity.AddComponent<dem::TextRenderer>();
    hudText->color = vmake(0.2f, 0.9f, 1.0f);
    hudText->transform = hudTransform;
    hudText->pCharSet = &font;
    hudText->Load();

    physics.earthTransform = earthTransform;
    physics.moonTransform = moonTransform;
    physics.timeWarp = 1.0f;

    float deltaTime = 0.0f;
    int frameCounter = 0;

    while(!dem::Renderer::Render()){
        frameCounter++;

        if(!gameOver){
            UpdateInput(deltaTime);

            physics.timeWarp = timeWarp;
            physics.Update(deltaTime);

            UpdateCamera();
            CheckLanding();
            UpdateHUD();
        } else {
            if(!deathStarted){
                deathStarted = true;
                deathTimer = 0.0f;
                deathCharIndex = 0;
                hudTransform->position = vmake(-0.25f, 0.0f, 0.0f);
                hudText->color = vmake(0.9f, 0.1f, 0.1f);
            }
            deathTimer += deltaTime;
            while(deathTimer > 0.08f && deathCharIndex < (int)deathMessage.size()){
                deathTimer -= 0.08f;
                deathCharIndex++;
            }
            hudText->SetText(deathMessage.substr(0, deathCharIndex) + "_");
        }

        dem::Input::Update();
        dem::Time::Update();
        deltaTime = dem::Time::GetDeltaTime();
    }

    logger->log("Total frames generated: ", frameCounter);
    return 0;
}
