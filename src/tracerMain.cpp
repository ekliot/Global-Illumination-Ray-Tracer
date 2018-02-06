/**
 * Main file for initializing and executing a ray tracing render of a 3D scene
 */
#include "stdafx.h"
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

// Include standard headers
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

// Include GLM vector
#include <glm/vec3.hpp>

#include <SFML/Window.hpp>

#include "Camera.h"
#include "PPlane.h"
#include "World.h"
using namespace glm;

// dimensions of drawing window
const int w_width  = 640;
const int w_height = 480;

// a pixel buffer of the window dimensions
uint8_t * pixels[(w_width*w_height)*4];

/**
 * Build the world scene and camera, then render the camera
 * into a pixel buffer
 */
void init() {
    World *world = new World();

    // add objects to the world

    // Camera cam  = new Camera(
    //     world,
    //     vec3( 1.0f, 2.53f -7.38f ), // pos TODO this is in world coords, should it be translated somehow?
    //     vec3( 0.0f ), // lookat TODO figure this one out
    //     vec3( 0.0f, 1.0f, 0.0f ), // up TODO figure this one out
    //     { w_width, w_height, 1.0f } // TODO wtf should the focal length be?
    // );

    // cam.render( pixels );
}

int main( void ) {

    // initialize the stuff we need for our rendering
    init();

    sf::Window window( sf::VideoMode( w_width, w_height ), "Ray Tracer" );

    return 0;
}
