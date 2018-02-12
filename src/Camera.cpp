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

Camera::Camera( World *w, vec3 p,    vec3 l, vec3 u, PPlane pp ) : \
                 world(w), pos(p), lookat(l),  up(u), plane(pp) {
    // constructor body
}

void Camera::render( png::image<png::rgb_pixel> negative ) {
    // trace and put stuff into the pixel buffer
    return negative;
}
