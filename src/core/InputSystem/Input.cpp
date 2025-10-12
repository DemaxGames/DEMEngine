#include "core/InputSystem/Input.h"

#include "core/Logger/Logger.h"

int keyState[dem::KeyCode::Slash+1][2];

float dem::Input::sensivity;

dem::math::vec2 mousePos[2]{dem::math::vec2(0.f, 0.f), dem::math::vec2(0.f, 0.f)};

void mouseCallBack(GLFWwindow* window, double xpos, double ypos){
    mousePos[0] = dem::math::vec2((float)xpos, (float)ypos);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
    switch(key){
        case GLFW_KEY_ESCAPE:
            keyState[dem::KeyCode::Esc][0] = action;
            break;
        case GLFW_KEY_Q:
            keyState[dem::KeyCode::Q][0] = action;
            break;
        case GLFW_KEY_W:
            keyState[dem::KeyCode::W][0] = action;
            break;
        case GLFW_KEY_E:
            keyState[dem::KeyCode::E][0] = action;
            break;
        case GLFW_KEY_R:
            keyState[dem::KeyCode::R][0] = action;
            break;
        case GLFW_KEY_T:
            keyState[dem::KeyCode::T][0] = action;
            break;
        case GLFW_KEY_Y:
            keyState[dem::KeyCode::Y][0] = action;
            break;
        case GLFW_KEY_U:
            keyState[dem::KeyCode::U][0] = action;
            break;
        case GLFW_KEY_I:
            keyState[dem::KeyCode::I][0] = action;
            break;
        case GLFW_KEY_O:
            keyState[dem::KeyCode::O][0] = action;
            break;
        case GLFW_KEY_P:
            keyState[dem::KeyCode::P][0] = action;
            break;
        case GLFW_KEY_LEFT_BRACKET:
            keyState[dem::KeyCode::Left_Brackets][0] = action;
            break;
        case GLFW_KEY_RIGHT_BRACKET:
            keyState[dem::KeyCode::Right_Brackets][0] = action;
            break;
        case GLFW_KEY_A:
            keyState[dem::KeyCode::A][0] = action;
            break;
        case GLFW_KEY_S:
            keyState[dem::KeyCode::S][0] = action;
            break;
        case GLFW_KEY_D:
            keyState[dem::KeyCode::D][0] = action;
            break;
        case GLFW_KEY_F:
            keyState[dem::KeyCode::F][0] = action;
            break;
        case GLFW_KEY_G:
            keyState[dem::KeyCode::G][0] = action;
            break;
        case GLFW_KEY_H:
            keyState[dem::KeyCode::H][0] = action;
            break;
        case GLFW_KEY_J:
            keyState[dem::KeyCode::J][0] = action;
            break;
        case GLFW_KEY_K:
            keyState[dem::KeyCode::K][0] = action;
            break;
        case GLFW_KEY_L:
            keyState[dem::KeyCode::L][0] = action;
            break;
        case GLFW_KEY_SEMICOLON:
            keyState[dem::KeyCode::Semicolon][0] = action;
            break;
        case GLFW_KEY_APOSTROPHE:
            keyState[dem::KeyCode::Prime][0] = action;
            break;
        case GLFW_KEY_Z:
            keyState[dem::KeyCode::Z][0] = action;
            break;
        case GLFW_KEY_X:
            keyState[dem::KeyCode::X][0] = action;
            break;
        case GLFW_KEY_C:
            keyState[dem::KeyCode::C][0] = action;
            break;
        case GLFW_KEY_V:
            keyState[dem::KeyCode::V][0] = action;
            break;
        case GLFW_KEY_B:
            keyState[dem::KeyCode::B][0] = action;
            break;
        case GLFW_KEY_N:
            keyState[dem::KeyCode::N][0] = action;
            break;
        case GLFW_KEY_M:
            keyState[dem::KeyCode::M][0] = action;
            break;
        case GLFW_KEY_COMMA:
            keyState[dem::KeyCode::Comma][0] = action;
            break;
        case GLFW_KEY_PERIOD:
            keyState[dem::KeyCode::Period][0] = action;
            break;
        case GLFW_KEY_SLASH:
            keyState[dem::KeyCode::Slash][0] = action;
            break;
    }
}

namespace dem{

int Input::Init(){
    for(int i = 0; i < KeyCode::Slash+1; i++){
        keyState[i][0] = GLFW_RELEASE;
        keyState[i][1] = GLFW_RELEASE;
    }
    glfwSetKeyCallback(Renderer::window, keyCallback);

    
    sensivity = 10.f;
    glfwSetInputMode(Renderer::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(Renderer::window, mouseCallBack);

    return 0;
}

void Input::Update(){
    for(int i = 0; i < KeyCode::Slash+1; i++){
        keyState[i][1] = keyState[i][0];
    }
    mousePos[1] = mousePos[0];
}

bool Input::GetKey(int keyCode){
    if(keyState[keyCode][0] == GLFW_PRESS || keyState[keyCode][0] == GLFW_REPEAT) return true;
    else return false;
}

bool Input::GetKeyDown(int keyCode){
    if(keyState[keyCode][0] == GLFW_PRESS && keyState[keyCode][1] == GLFW_RELEASE) return true;
    else return false;
}

bool Input::GetKeyUp(int keyCode){
    if(keyState[keyCode][0] == GLFW_RELEASE && keyState[keyCode][1] == GLFW_PRESS) return true;
    else return false;
}

float Input::GetAxisX(){
    return (mousePos[0][0] - mousePos[1][0]) * sensivity / 10.f;
}

float Input::GetAxisY(){
    return (mousePos[0][1] - mousePos[1][1]) * sensivity / 10.f;
}

}