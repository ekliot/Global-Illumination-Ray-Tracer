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
using namespace glm;

// dimensions of drawing window
const int I_WIDTH  = 960;
const int I_HEIGHT = 540;

void photo_print( png::image<png::rgb_pixel> negative, std::string filename ) {
    negative.write( filename );
}

/**
 * Build the world scene and camera, then render the camera
 * into a pixel buffer
 */
void init() {
    World *world = new World(
        vec4( 1.0f )
    );

    vec3 tri_a = vec3( 0.0f, 0.5f, 2.0f );
    vec3 tri_b = vec3( 0.0f, 0.0f, 2.0f );
    vec3 tri_c = vec3( 0.5f, 0.0f, 2.0f );
    Material tri_mat = {vec4( 0.0f )};

    // add objects to the world
    Triangle *tri = new Triangle( &tri_a, &tri_b, &tri_c, &tri_mat );

    world->add( tri );

    Camera *cam1 = new Camera(
        world,
        vec3( 0.0f ), // pos TODO this is in world coords, should it be translated somehow?
        vec3( 0.0f, 0.0f, 1.0f ), // lookat TODO figure this one out
        vec3( 0.0f, 1.0f, 0.0f ), // up TODO figure this one out
        { 1.0f, 1.0f, 1.0f } // TODO wtf should the focal length be?
        // { 1.92f, 1.08f, 200.0f } // TODO wtf should the focal length be?
    );

    // Camera *cam2 = new Camera(
    //     world,
    //     vec3( 0.0f ), // pos TODO this is in world coords, should it be translated somehow?
    //     vec3( 0.0f, 0.0f, 1.0f ), // lookat TODO figure this one out
    //     vec3( 0.0f, 1.0f, 0.0f ), // up TODO figure this one out
    //     { 1.92f, 1.08f, 400.0f } // TODO wtf should the focal length be?
    // );
    //
    // Camera *cam3 = new Camera(
    //     world,
    //     vec3( 0.0f ), // pos TODO this is in world coords, should it be translated somehow?
    //     vec3( 0.0f, 0.0f, 1.0f ), // lookat TODO figure this one out
    //     vec3( 0.0f, 1.0f, 0.0f ), // up TODO figure this one out
    //     { 1.92f, 1.08f, 100.0f } // TODO wtf should the focal length be?
    // );

    // Camera cam  = new Camera(
    //     world,
    //     vec3( 1.0f, 2.53f -7.38f ), // pos TODO this is in world coords, should it be translated somehow?
    //     vec3( 0.0f ), // lookat TODO figure this one out
    //     vec3( 0.0f, 1.0f, 0.0f ), // up TODO figure this one out
    //     { I_WIDTH, I_HEIGHT, 1.0f } // TODO wtf should the focal length be?
    // );

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
