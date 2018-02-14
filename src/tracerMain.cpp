/**
 * Main file for initializing and executing a ray tracing render of a 3D scene
 */
#if defined(_WIN32) || defined(_WIN64)
#include "stdafx.h"
#include <windows.h>
#endif

#include <cfloat>
#include <iostream>

// Include standard headers
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

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

    // =======================
    //          PLANE
    // =======================

    /*
        A----B
        |    |
        D----C
    */

    vec3 plane_cent = vec3( 1.8f, 1.24f, -4.57f );
    float plane_w = 1.2f;
    float plane_wt = 0.53f;
    float plane_l = 5.0f;
    float plane_lt = 1.18f;

    // TODO fixup the Object class to not mutate vectors in place so we don't need this wall of bullshit
    vec3 plane_a1 = plane_cent + vec3( -plane_w/plane_wt, 0.0f,  plane_l/plane_lt );
    vec3 plane_a2 = plane_cent + vec3( -plane_w/plane_wt, 0.0f,  plane_l/plane_lt );
    vec3 plane_b = plane_cent + vec3(  plane_w/plane_wt, 0.0f,  plane_l/plane_lt );
    vec3 plane_c1 = plane_cent + vec3(  plane_w/plane_wt, 0.0f, -plane_l/plane_lt );
    vec3 plane_c2 = plane_cent + vec3(  plane_w/plane_wt, 0.0f, -plane_l/plane_lt );
    vec3 plane_d = plane_cent + vec3( -plane_w/plane_wt, 0.0f, -plane_l/plane_lt );

    // two materials to differentiate
    Material plane1_mat = {vec4( 1.0f, 0.0f, 0.0f, 1.0f )};
    Material plane2_mat = {vec4( 1.0f, 0.0f, 0.0f, 1.0f )};

    // ABC
    Triangle* plane_tri1 = new Triangle( &plane_a1, &plane_b, &plane_c1, &plane1_mat );
    // ACD
    Triangle* plane_tri2 = new Triangle( &plane_a2, &plane_c2, &plane_d, &plane2_mat );

    world->add( plane_tri1 );
    world->add( plane_tri2 );

    vec3 sphere1_p = vec3( 0.77f, 2.7f, -5.0f );
    Material sphere1_mat = {vec4( 0.0f, 0.0f, 1.0f, 1.0f )};

    vec3 sphere2_p = vec3( 1.68f, 2.23f, -3.72f );
    Material sphere2_mat = {vec4( 0.0f, 1.0f, 0.0f, 1.0f )};

    float sphere_trans = 1.3f;
    float sphere_r = 0.55f * sphere_trans;

    Sphere* sphere1 = new Sphere( &sphere1_p, &sphere_r, &sphere1_mat );
    Sphere* sphere2 = new Sphere( &sphere2_p, &sphere_r, &sphere2_mat );

    world->add( sphere1 );
    world->add( sphere2 );

    // = = = = = = = = = = //
    // CAMERAS FROM ORIGIN //
    // = = = = = = = = = = //

    // // ORIGIN camera
    // Camera* cam1  = new Camera(
    //     world,
    //     vec3( 0.0f, 3.0f, -3.0f ), // pos
    //     vec3( 0.0f, -1.0f, 1.0f ), // lookat
    //     vec3( 0.0f, 1.0f, 1.0f ), // up
    //     { 1.92f, 1.08f, 1.0f }
    // );
    //
    // // camera upside down, looking forward
    // Camera* cam2  = new Camera(
    //     world,
    //     vec3( 0.0f ), // pos
    //     vec3( 0.0f, 0.0f, 1.0f ), // lookat
    //     vec3( 0.0f, -1.0f, 0.0f ), // up
    //     { 1.92f, 1.08f, 1.0f }
    // );
    //
    // // camera looking down
    // Camera* cam3  = new Camera(
    //     world,
    //     vec3( 0.0f ), // pos
    //     vec3( 0.0f, -1.0f, 0.0f ), // lookat
    //     vec3( 0.0f, 0.0f, 1.0f ), // up
    //     { 1.92f, 1.08f, 1.0f }
    // );

    // = = = = = = = = = = =  //
    // CAMERAS FOR CHECKPOINT //
    // = = = = = = = = = = =  //

    // main camera
    // Camera* cam1  = new Camera(
    //     world,
    //     vec3( 1.0f, 2.53f, -7.38f ), // pos
    //     vec3( 0.0f, 0.0f, 1.0f ), // lookat
    //     vec3( 0.0f, 1.0f, 0.0f ), // up
    //     { 1.92f/4, 1.08f/4, 0.2f }
    // );

    // camera looking down
    Camera* cam1  = new Camera(
        world,
        vec3( 1.0f, 8.53f, -7.38f ), // pos
        vec3( 0.0f, -1.0f, 0.0f ), // lookat
        vec3( 0.0f, 0.0f, 1.0f ), // up
        { 1.92f/4, 1.08f/4, 0.2f }
    );

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
