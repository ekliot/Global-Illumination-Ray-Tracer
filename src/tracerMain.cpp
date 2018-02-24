/**
 * Main file for initializing and executing a ray tracing render of a 3D scene
 */

#include <cfloat>
#include <iostream>

// Include GLM vector
#include <glm/vec3.hpp>

#include "glm/gtx/string_cast.hpp"

#include "png++/png.hpp"
#include "Camera.h"
#include "PPlane.h"
#include "World.h"
#include "Triangle.h"
#include "Sphere.h"
#include "Phong.h"
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
        vec3( 0.0f ), // background
        vec3( 0.1f )  // ambient light
    );

    vec3* light1_col = new vec3( 1.0f );
    vec3* light1_pos = new vec3( 1.1f, 4.52f, -5.7f );

    Light light1 = {
        light1_col, light1_pos
    };

    world->add_light( &light1 );

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
    vec3 plane_a = plane_cent + vec3( -plane_w/plane_wt, 0.0f,  plane_l/plane_lt );
    vec3 plane_a2 = plane_cent + vec3( -plane_w/plane_wt, 0.0f,  plane_l/plane_lt );
    vec3 plane_b = plane_cent + vec3(  plane_w/plane_wt, 0.0f,  plane_l/plane_lt );
    vec3 plane_c = plane_cent + vec3(  plane_w/plane_wt, 0.0f, -plane_l/plane_lt );
    vec3 plane_c2 = plane_cent + vec3(  plane_w/plane_wt, 0.0f, -plane_l/plane_lt );
    vec3 plane_d = plane_cent + vec3( -plane_w/plane_wt, 0.0f, -plane_l/plane_lt );

    // TODO implement these as Phong
    Phong* plane1_imodel = new Phong(
        vec3( 1.0f, 0.0f, 0.0f ), // obj color -- red
        vec3( 1.0f, 1.0f, 1.0f ), // specular color -- white
        // ka,  kd,   ks,   ke
        1.0f, 0.8f, 0.0f, 0.0f
    );
    Phong* plane2_imodel = new Phong(
        vec3( 0.0f, 0.0f, 1.0f ), // obj color -- blue
        vec3( 1.0f, 1.0f, 1.0f ), // specular color -- white
        // ka,  kd,   ks,   ke
        1.0f, 0.8f, 0.0f, 0.0f
    );

    // ABC
    Triangle* plane_tri1 = new Triangle( &plane_a, &plane_b, &plane_c, plane1_imodel );
    // ACD
    Triangle* plane_tri2 = new Triangle( &plane_a2, &plane_c2, &plane_d, plane2_imodel );

    world->add_object( plane_tri1 );
    world->add_object( plane_tri2 );

    vec3 sphere1_p = vec3( 0.77f, 2.7f, -5.0f );
    // TODO implement this as Phong
    Phong* sphere1_imodel = new Phong(
        vec3( 0.0f, 1.0f, 0.0f ),
        vec3( 1.0f, 1.0f, 1.0f ),
        // ka,  kd,   ks,   ke
        0.0f, 0.8f, 0.0f, 1.0f
    );

    // vec3 sphere2_p = vec3( 1.68f, 2.23f, -3.72f );
    // TODO implement this as Phong
    // Material sphere2_mat = {vec4( 0.0f, 1.0f, 0.0f, 1.0f )};

    float sphere_trans = 1.3f;
    float sphere_r = 0.55f * sphere_trans;

    Sphere* sphere1 = new Sphere( &sphere1_p, sphere_r, sphere1_imodel );
    // Sphere* sphere2 = new Sphere( &sphere2_p, &sphere_r, &sphere2_mat );

    world->add_object( sphere1 );
    // world->add( sphere2 );

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
    Camera* cam1  = new Camera(
        world,
        vec3( 1.0f, 2.53f, -7.38f ), // pos
        vec3( 0.0f, 0.0f, 1.0f ), // lookat
        vec3( 0.0f, 1.0f, 0.0f ), // up
        { 1.92f/4, 1.08f/4, 0.2f }
    );

    // camera looking down
    // Camera* cam1  = new Camera(
    //     world,
    //     vec3( 1.0f, 8.53f, -7.38f ), // pos
    //     vec3( 0.0f, -1.0f, 0.0f ), // lookat
    //     vec3( 0.0f, 0.0f, 1.0f ), // up
    //     { 1.92f/4, 1.08f/4, 0.2f }
    // );

    // PRINT IMAGES

    png::image<png::rgb_pixel> negative(I_WIDTH, I_HEIGHT);

    cam1->render( &negative );
    photo_print( negative, "etc/out1.png" );

    // cam2->render( &negative );
    // photo_print( negative, "out2.png" );
    //
    // cam3->render( &negative );
    // photo_print( negative, "out3.png" );

    delete cam1;
}

int main( void ) {

    // initialize the stuff we need for our rendering
    init();

    return 0;
}
