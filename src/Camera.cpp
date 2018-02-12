/**
 * Camera.cpp
 *
 * Camera object to render a world using ray tracing
 *
 * Author: ekliot
 */

#include "Camera.h"
#include "Ray.h"

using namespace png;
using glm::vec3;

Camera::Camera( World *w, vec3 p,    vec3 l, vec3 u, PPlane pp ) : \
                 world(w), pos(p), lookat(l),  up(u), plane(pp) {
}

image<rgb_pixel> Camera::render( image<rgb_pixel> negative ) {
    // Ray ray;
    vec4 colour;
    // trace and put stuff into the pixel buffer
    for ( size_t y = 0; y < negative.get_height(); ++y ) {
        for ( size_t x = 0; x < negative.get_width(); ++x ) {
            // ray = ;
            colour = vec4( 0.0f );
            negative[y][x] = rgb_pixel( x, y, x + y );
            // non-checking equivalent of image.set_pixel(x, y, ...);
        }
    }
    return negative;
}
