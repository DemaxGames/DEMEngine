#include "core/Engine/Object.h"

namespace dem{

Object::Object(){
    mesh.verticies = std::vector<math::vec3>();
    projection = math::mat4();
}

}
