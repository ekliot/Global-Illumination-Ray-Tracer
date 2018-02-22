/**
 * Camera.cpp
 *
 * Camera object to render a world using ray tracing
 *
 * Author: ekliot
 */

#include <iostream>

#include "glm/gtx/string_cast.hpp"

#include "Camera.h"
#include "Ray.h"

using namespace png;
using glm::vec3;

Camera::Camera( World *w, vec3 p,    vec3 l, vec3 u, PPlane pp ) : \
                 world(w), pos(p), lookat(l),  up(u), plane(pp) {}

void Camera::render( image<rgb_pixel> *negative ) {
    vec3 n = normalize( lookat );
    // vec3 n = normalize( pos - lookat ); // this is how it should be but we don't like it that way
    vec3 u = normalize( cross( up, n ) );
    vec3 v = normalize( cross( n, u ) );

    // std::cout << '\n';
    // std::cout << "u // { " << glm::to_string( u ) << " }" << '\n';
    // std::cout << "v // { " << glm::to_string( v ) << " }" << '\n';
    // std::cout << "n // { " << glm::to_string( n ) << " }" << '\n';

    // COL-MAJOR
    mat4 tmat = mat4(
        u.x,          v.x,          n.x,          0,
        u.y,          v.y,          n.y,          0,
        u.z,          v.z,          n.z,          0,
        dot(-pos, u), dot(-pos, v), dot(-pos, n), 1
    );

    // // ROW-MAJOR
    // mat4 tmat = mat4(
    //     u.x, u.y, u.z, -dot(pos, u),
    //     v.x, v.y, v.z, -dot(pos, v),
    //     n.x, n.y, n.z, -dot(pos, n),
    //       0,   0,   0,            1
    // );

    // std::cout << "tmat // { " << glm::to_string( tmat ) << " }" << '\n';
    // std::cout << '\n';
    // std::cout << "tmat[0] // { " << glm::to_string( tmat[0] ) << " }" << '\n';
    // std::cout << "tmat[1] // { " << glm::to_string( tmat[1] ) << " }" << '\n';
    // std::cout << "tmat[2] // { " << glm::to_string( tmat[2] ) << " }" << '\n';
    // std::cout << "tmat[3] // { " << glm::to_string( tmat[3] ) << " }" << '\n';
    // std::cout << '\n';

    world->transform_all( tmat );

    vec3 ray_ori = vec3( 0.0f );
    vec3 ray_dir;
    Ray *ray;
    vec4 colour;

    // width/height of a pixel in world coordinates
    float px_w = plane.w / negative->get_width();
    float px_h = plane.h / negative->get_height();

    // std::cout << "px:world // {"<< px_w << "," << px_h <<"}" << '\n';

    float start_x = -plane.w / 2;
    float start_y = plane.h / 2;

    float dir_x, dir_y, dir_z;

    // trace and put stuff into the pixel buffer
    for ( size_t y = 0; y < negative->get_height(); ++y ) {
        for ( size_t x = 0; x < negative->get_width(); ++x ) {
            dir_x = start_x + x * px_w + px_w/2;
            dir_y = start_y - y * px_h + px_h/2;
            dir_z = plane.foc_l;
            ray_dir = vec3( dir_x, dir_y, dir_z );

            // std::cout << "{" << dir_x << ":" << dir_y << ":" << dir_z  << "} -> ";

            ray = new Ray( &ray_ori, &ray_dir );

            // std::cout << glm::to_string( ray->direction )  << " -> [" << x << "," << y << "]" << '\n';

            colour = world->get_intersect( ray ) * 255.0f;
            negative->get_row(y)[x] = rgb_pixel(
                int( colour.x ),
                int( colour.y ),
                int( colour.z )
            );
        }
    }
}
