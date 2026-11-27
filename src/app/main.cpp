#include <iostream>
#include <stdio.h>
#include <filesystem>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include "core/dem.h"
#include "core/time/time.h"


#define PI 3.141592f

using namespace dem;

// ---------- small vector helpers ----------
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
static math::vec3 vnorm(const math::vec3& a){
    float l = vlen(a);
    if(l < 0.0001f) return vmake(0,0,0);
    return vscale(a, 1.0f / l);
}

// Make a quad face the camera (local +Z normal points at camera)
// x=pitch, y=yaw, z=roll for the engine rotation convention
static void faceCamera(math::vec3& rot, const math::vec3& objPos, const math::vec3& camPos){
    math::vec3 dir = vnorm(vsub(camPos, objPos));
    rot.x = -std::asin(dir.data[1]);
    rot.y = std::atan2(dir.data[0], dir.data[2]);
    rot.z = 0.0f;
}

// ---------- globals ----------
ecs::Entity earthEntity, moonEntity, csmEntity, cameraEntity, lightEntity, hudEntity;
ecs::Entity starEntity;
ecs::Entity plumeEntity;
ecs::Entity markerEntities[6]; // prograde, retrograde, radial_out, radial_in, normal, antinormal

TransformComponent* earthTransform = nullptr;
TransformComponent* moonTransform = nullptr;
TransformComponent* csmTransform = nullptr;
TransformComponent* cameraTransform = nullptr;
TransformComponent* starTransform = nullptr;
TransformComponent* plumeTransform = nullptr;
TransformComponent* markerTransforms[6];
TransformComponent* hudTransform = nullptr;

CameraComponent* cameraComponent = nullptr;
Rigidbody* csmRigidbody = nullptr;
TextRenderer* hudText = nullptr;

PhysicsSystem physics;

// meshes / textures
Mesh sphereMesh, csmMesh, quadMesh;
Image earthTexture, moonTexture, csmTexture, starsTexture, plumeTexture;
Image markerTextures[6];
Material* earthMat = nullptr;
Material* moonMat = nullptr;
Material* csmMat = nullptr;
Material* quadMat = nullptr;
Material* starsMat = nullptr;
Material* plumeMat = nullptr;
Material* markerMats = nullptr;
CharSet font;

// game state
float thrust = 0.0f;
const float maxThrust = 2000.0f;
const float thrustStep = 800.0f;
float timeWarp = 1.0f;
bool showOrbitInfo = false;
bool gameOver = false;
bool landed = false;

// camera
float cameraDistance = 120.0f;
float cameraYaw = PI * 0.5f;
float cameraPitch = 0.2f;

const float t_earthRadius = 6000.0f;
const float t_moonRadius = 2000.0f;
const float t_moonDistance = 380000.0f;

// death screen
bool deathStarted = false;
float deathTimer = 0.0f;
int deathCharIndex = 0;
std::string deathMessage = "MISSION FAILED";

// marker names
const char* markerNames[6] = {
    "PROGRADE", "RETROGRADE", "RADIAL OUT", "RADIAL IN", "NORMAL", "ANTI-NORMAL"
};

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
    return dv * mass / thrust;
}

// ---------- input ----------
void UpdateInput(float deltaTime){
    if(dem::Input::GetKey(dem::KeyCode::LShift) || dem::Input::GetKey(dem::KeyCode::RShift)){
        thrust += thrustStep * deltaTime;
        if(thrust > maxThrust) thrust = maxThrust;
    }
    if(dem::Input::GetKey(dem::KeyCode::LCtrl) || dem::Input::GetKey(dem::KeyCode::RCtrl)){
        thrust -= thrustStep * deltaTime;
        if(thrust < 0.0f) thrust = 0.0f;
    }
    if(dem::Input::GetKeyDown(dem::KeyCode::Z)) thrust = maxThrust;
    if(dem::Input::GetKeyDown(dem::KeyCode::X)) thrust = 0.0f;

    csmTransform->GetModelMatrix();

    if(thrust > 0.0f && timeWarp <= 1.0f && !landed){
        csmRigidbody->AddForce(vscale(csmTransform->up, thrust));
    }

    if(timeWarp <= 1.0f && !landed){
        float rot = 1.0f * deltaTime;
        if(dem::Input::GetKey(dem::KeyCode::S)) csmTransform->RotateLocal(rot, dem::math::vec3(1.0, 0.0, 0.0));
        if(dem::Input::GetKey(dem::KeyCode::W)) csmTransform->RotateLocal(-rot, dem::math::vec3(1.0, 0.0, 0.0));
        if(dem::Input::GetKey(dem::KeyCode::E)) csmTransform->RotateLocal(rot, dem::math::vec3(0.0, 1.0, 0.0));
        if(dem::Input::GetKey(dem::KeyCode::Q)) csmTransform->RotateLocal(-rot, dem::math::vec3(0.0, 1.0, 0.0));
        if(dem::Input::GetKey(dem::KeyCode::A)) csmTransform->RotateLocal(rot, dem::math::vec3(0.0, 0.0, 1.0));
        if(dem::Input::GetKey(dem::KeyCode::D)) csmTransform->RotateLocal(-rot, dem::math::vec3(0.0, 0.0, 1.0));
    }

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

    static bool moonSoI = false;

    if(timeWarp > 1.0f){
        float alt = physics.GetAltitudeAboveEarth(csmTransform->position);
        float distMoon = vlen(vsub(moonTransform->position, csmTransform->position));
        if(alt < physics.atmosphereTop){
            timeWarp = 1.0f;
        }
        bool nowInMoonSoI = distMoon < physics.moonSoI;
        if(nowInMoonSoI && !moonSoI){
            // just entered Moon SoI: disable warp
            moonSoI = true;
            timeWarp = 1.0f;
        } else if(!nowInMoonSoI){
            // left Moon SoI: warp can be re-enabled
            moonSoI = false;
        }
    }

    if(dem::Input::GetKeyDown(dem::KeyCode::H)){
        showOrbitInfo = !showOrbitInfo;
    }

    float wheel = dem::Input::GetAxisWheel();
    if(wheel < 0.0f) cameraDistance *= 1.1f;
    if(wheel > 0.0f) cameraDistance *= 0.9f;
    if(cameraDistance < 1.0f) cameraDistance = 1.0f;
    if(cameraDistance > 30000.0f) cameraDistance = 30000.0f;

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

    // also update starfield center to follow camera (skybox effect)
    if(starTransform) starTransform->position = camPos;

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
    if(alt > 5.0f) return;
    if(alt < -bodyRadius) return;

    math::vec3 normal = (dist > 0.0001f) ? vscale(rel, 1.0f / dist) : vmake(0,1,0);
    float verticalSpeed = vdot(csmRigidbody->velocity, normal);

    if(verticalSpeed <= -4.0f){
        gameOver = true;
        return;
    }
    if(verticalSpeed > -4.0f){
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

// ---------- plume ----------
void UpdatePlume(){
    if(!plumeTransform) return;
    if(thrust > 0.0f && timeWarp <= 1.0f && !landed){
        plumeTransform->position = math::vec3(0.0f, -7.3f, 0.0);

        // float zLen = 8.0f + (thrust / maxThrust) * 90.0f;
        // plumeTransform->scale = vmake(10.0f, 10.0f, zLen);

    } else {
        plumeTransform->scale = vmake(1.f, 1.f, 0.0001f);
    }
}

// ---------- orbit markers ----------
void UpdateMarkers(){
    // Determine which body we reference for orbital directions
    math::vec3 relEarth = vsub(csmTransform->position, earthTransform->position);
    float distEarth = vlen(relEarth);
    float distMoon = vlen(vsub(csmTransform->position, moonTransform->position));
    bool inMoon = distMoon < physics.moonSoI;

    math::vec3 bodyCenter = inMoon ? moonTransform->position : earthTransform->position;
    float bodyRadius = inMoon ? t_moonRadius : t_earthRadius;

    math::vec3 relPos = vsub(csmTransform->position, bodyCenter);
    math::vec3 radialOut = vnorm(relPos);
    math::vec3 radialIn = vscale(radialOut, -1.0f);

    math::vec3 vel = csmRigidbody->velocity;
    math::vec3 prograde = vnorm(vel);
    math::vec3 retrograde = vscale(prograde, -1.0f);

    math::vec3 h = vcross(relPos, vel);
    math::vec3 normal = vnorm(h);
    math::vec3 antiNormal = vscale(normal, -1.0f);

    math::vec3 dirs[6] = { prograde, retrograde, radialOut, radialIn, normal, antiNormal };
    float markerDist = 120.0f;

    for(int i = 0; i < 6; i++){
        if(!markerTransforms[i]) continue;
        math::vec3 mPos = vadd(csmTransform->position, vscale(dirs[i], markerDist));
        markerTransforms[i]->position = mPos;
        faceCamera(markerTransforms[i]->rotation, mPos, cameraTransform->position);
        markerTransforms[i]->scale = vmake(8.0f, 8.0f, 8.0f);
    }
}

// ---------- HUD ----------
void UpdateHUD(){
    if(gameOver){
        return;
    }

    std::string text;
    if(showOrbitInfo){
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
        text += "WASD: pitch/yaw   QE: roll   Wheel: zoom   Mouse: camera\n";
        math::mat4 plumeMatrix = plumeTransform->GetModelMatrix();
        math::vec4 plumePosition = plumeMatrix[3];
        text += "Plume: " + std::to_string(plumePosition[0]) + " " + std::to_string(plumePosition[1]) + " " + std::to_string(plumePosition[2]) + "\n";
        text += "CSM: " + std::to_string(csmTransform->position.x) + " " + std::to_string(csmTransform->position.y) + " " + std::to_string(csmTransform->position.z) + "\n";

        std::transform(text.begin(), text.end(), text.begin(), ::toupper);
    }

    hudText->SetText(text);
}

// ---------- main ----------
int main(){
    Logger *logger = Logger::get();

    ecs::init();
    ecs::RegisterComponent<TransformComponent>();
    ecs::RegisterComponent<CameraComponent>();
    ecs::RegisterComponent<MeshRenderer>();
    ecs::RegisterComponent<TextRenderer>();
    ecs::RegisterComponent<LightSourceComponent>();
    ecs::RegisterComponent<Rigidbody>();

    Renderer::Init(1920, 1080);
    Input::Init();
    Input::sensivity = 1.0f;

    font.Load("examples/font.txt");

    sphereMesh.Load("assets/sphere.obj");
    csmMesh.Load("assets/CSM.obj");
    quadMesh.Load("assets/quad.obj");

    earthTexture.Load("assets/earth.png");
    moonTexture.Load("assets/moon.png");
    csmTexture.Load("assets/csm.png");
    starsTexture.Load("assets/stars.png");
    plumeTexture.Load("assets/cat.png");
    markerTextures[0].Load("assets/prograde.png");
    markerTextures[1].Load("assets/retrograde.png");
    markerTextures[2].Load("assets/radial_out.png");
    markerTextures[3].Load("assets/radial_in.png");
    markerTextures[4].Load("assets/normal.png");
    markerTextures[5].Load("assets/antinormal.png");


    earthMat = new Material();
    moonMat = new Material();
    csmMat = new Material();
    quadMat = new Material();
    starsMat = new Material();
    plumeMat = new Material();
    markerMats = new Material[6]();

    earthMat->glImage.image = &earthTexture;
    earthMat->Load();
    moonMat->glImage.image = &moonTexture;
    moonMat->Load();
    csmMat->glImage.image = &csmTexture;
    csmMat->Load();
    quadMat->glImage.image = &plumeTexture; // quadMat uses plume tex for markers/plume (markers use their own below)
    quadMat->Load();
    starsMat->glImage.image = &starsTexture;
    starsMat->Load();
    plumeMat->fragment.sourceFile = "assets/marker.frag";
    plumeMat->glImage.image = &plumeTexture;
    plumeMat->Load();
    for(int i = 0; i < 6; i++){
        markerMats[i].fragment.sourceFile = "assets/marker.frag";
        markerMats[i].glImage.image = &(markerTextures[i]);
        markerMats[i].Load();
    }

    // NOTE: markers need individual textures but share quadMat's shader program.
    // For now all markers use plume.png; you can swap per-marker material later.

    // Starfield (drawn first so it's the background)
    starEntity = ecs::CreateEntity();
    starTransform = starEntity.AddComponent<TransformComponent>();
    starTransform->position = vmake(0,0,0);
    starTransform->scale = vmake(1000000.0f, 1000000.0f, 1000000.0f);
    {
        MeshRenderer* r = starEntity.AddComponent<MeshRenderer>();
        r->mesh = &sphereMesh; r->material = starsMat; r->Load();
    }

    // Earth
    earthEntity = ecs::CreateEntity();
    earthTransform = earthEntity.AddComponent<TransformComponent>();
    earthTransform->position = vmake(0,0,0);
    earthTransform->scale = vmake(t_earthRadius, t_earthRadius, t_earthRadius);
    {
        MeshRenderer* r = earthEntity.AddComponent<MeshRenderer>();
        r->mesh = &sphereMesh; r->material = earthMat; r->Load();
    }

    // Moon
    moonEntity = ecs::CreateEntity();
    moonTransform = moonEntity.AddComponent<TransformComponent>();
    moonTransform->position = vmake(t_moonDistance, 0, 0);
    moonTransform->scale = vmake(t_moonRadius, t_moonRadius, t_moonRadius);
    {
        MeshRenderer* r = moonEntity.AddComponent<MeshRenderer>();
        r->mesh = &sphereMesh; r->material = moonMat; r->Load();
    }

    // CSM
    csmEntity = ecs::CreateEntity();
    csmTransform = csmEntity.AddComponent<TransformComponent>();
    csmTransform->position = vmake(t_earthRadius + 100.0f, 0.0f, 0.0f);
    csmTransform->rotation = vmake(0.0f, -PI * 0.5f, -PI * 0.5f);
    csmTransform->scale = vmake(2.0f, 2.0f, 2.0f);
    csmRigidbody = csmEntity.AddComponent<Rigidbody>();
    csmRigidbody->mass = 100.0f;
    csmRigidbody->inverseMass = 1.0f / csmRigidbody->mass;
    csmRigidbody->useGravity = true;
    csmRigidbody->isKinematic = false;
    {
        MeshRenderer* r = csmEntity.AddComponent<MeshRenderer>();
        r->mesh = &csmMesh; r->material = csmMat; r->Load();
    }

    // Engine plume
    plumeEntity = ecs::CreateEntity();
    plumeTransform = plumeEntity.AddComponent<TransformComponent>();
    plumeTransform->parent = csmTransform;
    plumeTransform->position = vmake(0,0,0);
    plumeTransform->scale = vmake(0.001f, 0.001f, 0.001f);
    {
        MeshRenderer* r = plumeEntity.AddComponent<MeshRenderer>();
        r->mesh = &quadMesh; r->material = plumeMat; r->Load();
    }

    // Orbit markers (6 billboards)
    for(int i = 0; i < 6; i++){
        markerEntities[i] = ecs::CreateEntity();
        markerTransforms[i] = markerEntities[i].AddComponent<TransformComponent>();
        markerTransforms[i]->position = vmake(0,0,0);
        markerTransforms[i]->scale = vmake(8.0f, 8.0f, 8.0f);
        {
            MeshRenderer* r = markerEntities[i].AddComponent<MeshRenderer>();
            r->mesh = &quadMesh;
            r->material = &markerMats[i];
            r->Load();
        }
    }

    // Camera
    cameraEntity = ecs::CreateEntity();
    cameraTransform = cameraEntity.AddComponent<TransformComponent>();
    cameraTransform->position = vmake(t_earthRadius + 220.0f, 0.0f, 0.0f);
    cameraComponent = cameraEntity.AddComponent<CameraComponent>();
    cameraComponent->pTransform = cameraTransform;
    cameraComponent->fov = PI * 0.5f;
    cameraComponent->nearClip = 1.0f;
    cameraComponent->farClip = 2000000.0f;
    cameraComponent->proj = dem::Perspective;

    // Light
    lightEntity = ecs::CreateEntity();
    {
        TransformComponent* lt = lightEntity.AddComponent<TransformComponent>();
        lt->position = vmake(-5000.f, 3000.f, -5000.f);
        LightSourceComponent* l = lightEntity.AddComponent<LightSourceComponent>();
        l->transform = lt;
        l->color = vmake(1.f, 1.f, 1.f);
        l->strength = 1.0f;
    }

    // HUD text
    hudEntity = ecs::CreateEntity();
    hudTransform = hudEntity.AddComponent<TransformComponent>();
    hudTransform->position = vmake(-0.48f, 15.5f, 0.0f);
    hudText = hudEntity.AddComponent<TextRenderer>();
    hudText->color = vmake(0.2f, 0.9f, 1.0f);
    hudText->transform = hudTransform;
    hudText->pCharSet = &font;
    hudText->Load();

    physics.earthTransform = earthTransform;
    physics.moonTransform = moonTransform;
    physics.timeWarp = 1.0f;

    float deltaTime = 0.0f;
    int frameCounter = 0;

    while(!Renderer::Render()){
        frameCounter++;

        if(!gameOver){
            UpdateInput(deltaTime);

            physics.timeWarp = timeWarp;
            physics.Update(deltaTime);

            UpdateCamera();
            CheckLanding();
            UpdatePlume();
            UpdateMarkers();
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

        Input::Update();
        Time::Update();
        deltaTime = Time::GetDeltaTime();
    }

    logger->log("Total frames generated: ", frameCounter);
    return 0;
}
