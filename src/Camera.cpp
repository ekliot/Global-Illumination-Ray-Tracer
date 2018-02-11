/**
 * Camera.cpp
 *
 * Camera object to render a world using ray tracing
 *
 * Author: ekliot
 */
//#include "stdafx.h"

#include "Camera.h"

using namespace glm;

Camera::Camera( World *w, vec3 p,    vec3 l, vec3 up, PPlane pp ) : \
                 world(w), pos(p), lookat(l),  up(up), plane(pp) {
    // constructor body
}

void Camera::render( uint8_t *px_buf ) {
    // trace and put stuff into the pixel buffer
    //return NULL;
}
