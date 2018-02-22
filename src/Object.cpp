#include <glm/matrix.hpp>

#include "Object.h"

using namespace glm;

Object::Object( IlluminationModel* _imodel ) : imodel(_imodel) {}

void Object::transform( mat4 matrix ) {}

IlluminationModel* Object::get_imodel() {
    return imodel;
}

vec3 Object::convert( vec4 vec ) {
    return vec3( vec.x / vec.w, vec.y / vec.w, vec.z / vec.w );
}
