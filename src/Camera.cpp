/**
 * Camera.cpp
 *
 * Camera object to render a world using ray tracing
 *
 * Author: ekliot
 */

#include <iostream>

#include "Camera.h"
#include "Ray.h"

using namespace png;
using glm::vec3;

Camera::Camera( World *w, vec3 p,    vec3 l, vec3 u, PPlane pp ) : \
                 world(w), pos(p), lookat(l),  up(u), plane(pp) {}

void Camera::render( image<rgb_pixel> *negative ) {
    // vec3 n = this->pos - this->lookat;
    // vec3 u = cross(this->up, n);
    // u = normalize(u);
    // vec3 v = cross(n, u);
    // mat4 tmat = mat4(
    //     u.x, v.x, n.x, 0,
    //     u.y, v.y, n.y, 0,
    //     u.z, v.y, n.z, 0,
    //     - dot(pos, u), -dot(pos, v), -dot(pos, n),1);
    //
    // world->transform_all_to_ccoord(tmat);

    vec3 ray_ori;
    vec3 ray_dir;
    Ray *ray;
    vec4 colour;

    float px_w = plane.w / negative->get_width();
    float px_h = plane.h / negative->get_height();

    std::cout << "{"<< px_w << "," << px_h <<"}" << '\n';

    float start_x = -plane.w / 2;
    float start_y = plane.h / 2;

    float dir_x, dir_y, dir_z;

    // trace and put stuff into the pixel buffer
    for ( size_t y = 0; y < negative->get_height(); ++y ) {
        for ( size_t x = 0; x < negative->get_width(); ++x ) {
            ray_ori = pos;
            dir_x = start_x + x * px_w + px_w/2;
            dir_y = start_y - y * px_h + px_h/2;
            dir_z = plane.foc_l;
            // std::cout << "{" << dir_x << ":" << dir_y << ":" << dir_z  << "} -> ";
            ray_dir = vec3( dir_x, dir_y, dir_z );
            ray = new Ray( &ray_ori, &ray_dir );
            // std::cout << "{" << ray->direction->x << ":" << ray->direction->y << ":" << ray->direction->z << "} -> [" << x << "," << y << "]" << '\n';

            colour = world->get_intersect( ray );
            negative->get_row(y)[x] = rgb_pixel( int( colour.x*255 ), int( colour.y*255 ), int( colour.z*255 ) );
        }
    }
}
