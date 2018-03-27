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

Camera::Camera( World* _w, vec3 _pos, vec3 _look, vec3 _up, PPlane _pp ) : world(_w), plane(_pp) {
    vec3 n = normalize( _look );
    // vec3 n = normalize( pos - look ); // this is how it should be but we don't like it that way
    vec3 u = normalize( cross( _up, n ) );
    vec3 v = normalize( cross( n, u ) );

    // COL-MAJOR
    transform_mat = mat4(
        u.x,           v.x,           n.x,           0,
        u.y,           v.y,           n.y,           0,
        u.z,           v.z,           n.z,           0,
        dot(-_pos, u), dot(-_pos, v), dot(-_pos, n), 1
    );

    reverse_transform_mat = inverse(transform_mat);

    // // ROW-MAJOR
    // transform_mat = mat4(
    //     u.x, u.y, u.z, -dot(pos, u),
    //     v.x, v.y, v.z, -dot(pos, v),
    //     n.x, n.y, n.z, -dot(pos, n),
    //       0,   0,   0,            1
    // );
}

Camera::~Camera() {
    delete world;
}

void Camera::set_scene() {
    if ( !is_set ) {
        world->transform_all( transform_mat );
        is_set = true;
    }
}

void Camera::break_scene() {
    if ( is_set ){
        world->transform_all( inverse( transform_mat ) );
        is_set = false;
    }
}

void Camera::render( image<rgb_pixel>* negative, uint ss_rate ) {

    if ( !is_set ) {
        std::cout << "Camera::render() called without setting the scene first" << '\n';
        return;
    }

    vec3 ray_ori = vec3( 0.0f );
    vec3 ray_dir;
    Ray *ray;
    vec3 color;

    // width/height of a pixel in world coordinates
    float px_w = plane.w / negative->get_width();
    float px_h = plane.h / negative->get_height();

    // we start at the top-left
    float start_x = -plane.w / 2;
    float start_y = plane.h / 2;

    float dir_x, dir_y, dir_z;

    // trace and put stuff into the pixel buffer
    for ( size_t y = 0; y < negative->get_height(); ++y ) {
        for ( size_t x = 0; x < negative->get_width(); ++x ) {

            // if we have a default ss_rate, don't bother with it
            if ( !ss_rate ) {
                dir_x = start_x + x * px_w + px_w/2;
                dir_y = start_y - y * px_h - px_h/2;
                dir_z = plane.foc_l;

                ray_dir = vec3( dir_x, dir_y, dir_z );
                ray = new Ray( &ray_ori, &ray_dir );

                color = world->get_intersect( ray ) * 255.0f;
                negative->get_row(y)[x] = rgb_pixel(
                    int( color.x ),
                    int( color.y ),
                    int( color.z )
                );

                delete ray;
            }
            // otherwise, engage super sampling
            else {
                // how many sub-pixels in each direction
                uint x_div = ((ss_rate % 2) + ss_rate)/2 + 1;
                uint y_div = x_div - ss_rate % 2;

                float r = 0.0f;
                float g = 0.0f;
                float b = 0.0f;

                float base_x = start_x + x * px_w;
                float base_y = start_y - y * px_h;

                for ( size_t _x = 1; _x <= x_div*2; _x+=2 ) {
                    for ( size_t _y = 1; _y <= y_div*2; _y+=2 ) {
                        dir_x = base_x + _x * px_w/(x_div*2);
                        dir_y = base_y - _y * px_h/(y_div*2);
                        dir_z = plane.foc_l;

                        ray_dir = vec3( dir_x, dir_y, dir_z );
                        ray = new Ray( &ray_ori, &ray_dir );

                        color = world->get_intersect( ray );
                        r += color.x;
                        g += color.y;
                        b += color.z;

                        delete ray;
                    }
                }

                uint samples = x_div * y_div;

                negative->get_row(y)[x] = rgb_pixel(
                    int( 255.0f * r / static_cast<float>(samples) ),
                    int( 255.0f * g / static_cast<float>(samples) ),
                    int( 255.0f * b / static_cast<float>(samples) )
                );
            }
        }
    }
}
