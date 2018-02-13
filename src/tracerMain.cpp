/**
 * Main file for initializing and executing a ray tracing render of a 3D scene
 */
#if defined(_WIN32) || defined(_WIN64)
#include "stdafx.h"
#include <windows.h>
#endif

// Include standard headers
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <iostream>

// Include GLM vector
#include <glm/vec3.hpp>

#include "png++/png.hpp"
#include "Camera.h"
#include "PPlane.h"
#include "World.h"
#include "Triangle.h"
#include "Sphere.h"
using namespace glm;

// dimensions of drawing window
const int I_WIDTH  = 1920;
const int I_HEIGHT = 1080;

void photo_print( png::image<png::rgb_pixel> negative, std::string filename ) {
    negative.write( filename );
}

/**
 * Build the world scene and camera, then render the camera
 * into a pixel buffer
 */
void init() {
    World *world = new World(
        vec4( 0.0f )
    );

    // add objects to the world

    // PLANE
    vec3 plane_cent = vec3( 2.11f, 1.24f, -4.57f );
    float plane_w = 1.0f;
    float plane_wt = 0.53f;
    float plane_l = 1.0f;
    float plane_lt = 1.18f;

    vec3 plane_a = plane_cent + vec3( -plane_w/plane_wt, 0.0f,  plane_l/plane_lt );
    vec3 plane_b = plane_cent + vec3(  plane_w/plane_wt, 0.0f,  plane_l/plane_lt );
    vec3 plane_c = plane_cent + vec3(  plane_w/plane_wt, 0.0f, -plane_l/plane_lt );
    vec3 plane_d = plane_cent + vec3( -plane_w/plane_wt, 0.0f, -plane_l/plane_lt );
    Material plane_mat = {vec4( 1.0f, 0.0f, 0.0f, 1.0f )};

    Triangle* plane_tri1 = new Triangle( &plane_a, &plane_b, &plane_c, &plane_mat );
    Triangle* plane_tri2 = new Triangle( &plane_a, &plane_c, &plane_d, &plane_mat );

    world->add( plane_tri1 );
    world->add( plane_tri2 );

    // vec3 sphere1_p = vec3( 0.77f, 2.7f, -5.0f );
    vec3 sphere1_p = vec3( 0.0f, 0.0f, 2.0f );

    // vec3 sphere2_p = vec3( 1.68f, 2.23f, -3.72f );
    // float sphere2_r = 1.3f;

    float sphere_trans = 1.3f;
    float sphere_r = 0.5f * sphere_trans;
    Material sphere_mat = {vec4( 1.0f )};

    Sphere* sphere1 = new Sphere( &sphere1_p, &sphere_r, &sphere_mat );
    // Sphere* sphere2 = new Sphere( &sphere2_p, &sphere_r, &sphere_mat );

    world->add( sphere1 );
    // world->add( sphere2 );

    // = = = = = = = = = = //
    // CAMERAS FROM ORIGIN //
    // = = = = = = = = = = //

    // ORIGIN camera
    Camera* cam1  = new Camera(
        world,
        vec3( 0.0f ), // pos TODO this is in world coords, should it be translated somehow?
        vec3( 0.0f, 0.0f, 1.0f ), // lookat TODO figure this one out
        vec3( 0.0f, 1.0f, 0.0f ), // up TODO figure this one out
        { 1.92f, 1.08f, 1.0f } // TODO wtf should the focal length be?
    );
    //
    // // camera upside down, looking forward
    // Camera* cam2  = new Camera(
    //     world,
    //     vec3( 0.0f ), // pos TODO this is in world coords, should it be translated somehow?
    //     vec3( 0.0f, 0.0f, 1.0f ), // lookat TODO figure this one out
    //     vec3( 0.0f, -1.0f, 0.0f ), // up TODO figure this one out
    //     { 1.92f, 1.08f, 1.0f } // TODO wtf should the focal length be?
    // );
    //
    // // camera looking down
    // Camera* cam3  = new Camera(
    //     world,
    //     vec3( 0.0f ), // pos TODO this is in world coords, should it be translated somehow?
    //     vec3( 0.0f, -1.0f, 0.0f ), // lookat TODO figure this one out
    //     vec3( 0.0f, 0.0f, 1.0f ), // up TODO figure this one out
    //     { 1.92f, 1.08f, 1.0f } // TODO wtf should the focal length be?
    // );

    // = = = = = = = = = = =  //
    // CAMERAS FOR CHECKPOINT //
    // = = = = = = = = = = =  //

    // main camera
    // Camera* cam1  = new Camera(
    //     world,
    //     vec3( 1.0f, 2.53f, -7.38f ), // pos TODO this is in world coords, should it be translated somehow?
    //     vec3( 0.0f, 0.0f, 1.0f ), // lookat TODO figure this one out
    //     vec3( 0.0f, 1.0f, 0.0f ), // up TODO figure this one out
    //     { 1.92f, 1.08f, 1.0f } // TODO wtf should the focal length be?
    // );

    // // camera upside down, looking forward
    // Camera* cam2  = new Camera(
    //     world,
    //     vec3( 1.0f, 2.53f, -7.38f ), // pos TODO this is in world coords, should it be translated somehow?
    //     vec3( 0.0f, 0.0f, 1.0f ), // lookat TODO figure this one out
    //     vec3( 0.0f, -1.0f, 0.0f ), // up TODO figure this one out
    //     { 1.92f, 1.08f, 1.0f } // TODO wtf should the focal length be?
    // );
    //
    // // camera looking down
    // Camera* cam3  = new Camera(
    //     world,
    //     vec3( 1.0f, 2.53f, -7.38f ), // pos TODO this is in world coords, should it be translated somehow?
    //     vec3( 0.0f, -1.0f, 0.0f ), // lookat TODO figure this one out
    //     vec3( 0.0f, 0.0f, 1.0f ), // up TODO figure this one out
    //     { 1.92f, 1.08f, 1.0f } // TODO wtf should the focal length be?
    // );

    // PRINT IMAGES

    png::image<png::rgb_pixel> negative(I_WIDTH, I_HEIGHT);

    cam1->render( &negative );
    photo_print( negative, "out1.png" );

    // cam2->render( &negative );
    // photo_print( negative, "out2.png" );
    //
    // cam3->render( &negative );
    // photo_print( negative, "out3.png" );

}

int main( void ) {

    // initialize the stuff we need for our rendering
    init();

    return 0;
}
