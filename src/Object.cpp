#include "stdafx.h"
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#include <glm/matrix.hpp>

#include "Object.h"

using namespace glm;

Object::Object( Material* m ) : material(m) {}

void Object::transform( mat4 matrix ) {}

Material Object::getMaterial() {
    return *material;
}

vec3 Object::convert( vec4 vec ) {
    return vec3( vec.x / vec.w, vec.y / vec.w, vec.z / vec.w );
}
