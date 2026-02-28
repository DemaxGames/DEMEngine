#pragma once

#include "core/InputSystem/KeyCodes.h"

namespace dem{
namespace Input{

extern float sensivity;

int Init();
void Update();
bool GetKey(int keyCode);
bool GetKeyDown(int keyCode);
bool GetKeyUp(int keyCode);
float GetAxisX();
float GetAxisY();
}
}