/**
 * Main file for initializing and executing a ray tracing render of a 3D scene
 */

#include <cfloat>
#include <ctime>
#include <iostream>

// Include GLM vector
#include <glm/vec3.hpp>

#include "glm/gtx/string_cast.hpp"

#include "png++/png.hpp"
#include "Camera.h"
#include "CheckerBoard.h"
#include "PPlane.h"
#include "Phong.h"
#include "Rectangle.h"
#include "SolidMaterial.h"
#include "Sphere.h"
#include "Triangle.h"
#include "World.h"

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

}

void cornell_box() {
    World *world = new World(
        vec3( 0.0f ), // background
        vec3( 0.1f )  // ambient light
    );

    // add objects to the world

    // =======================
    //          CUBE
    // =======================

    /*
            E--------F
           /|      / |
         /  |    /   |
        A---H---B----G
        |  /    |   /
        |/      | /
        D-------C
    */

    float cube_w = 1.0f;
    float cube_r = cube_w / 2;
    vec3 cube_center = vec3( 0.0f );

    // llf = lower left far
    vec3 pt_a = cube_center + vec3( -cube_r,  cube_r, -cube_r );
    vec3 pt_b = cube_center + vec3(  cube_r,  cube_r, -cube_r );
    vec3 pt_c = cube_center + vec3(  cube_r, -cube_r, -cube_r );
    vec3 pt_d = cube_center + vec3( -cube_r, -cube_r, -cube_r );
    vec3 pt_e = cube_center + vec3( -cube_r,  cube_r,  cube_r );
    vec3 pt_f = cube_center + vec3(  cube_r,  cube_r,  cube_r );
    vec3 pt_g = cube_center + vec3(  cube_r, -cube_r,  cube_r );
    vec3 pt_h = cube_center + vec3( -cube_r, -cube_r,  cube_r );

    Phong* plane_imodel;
    SolidMaterial* plane_mat;
    Rectangle* rect;

    //
    // floor
    //

    plane_imodel = new Phong(
        vec3( 1.0f, 1.0f, 1.0f ), // specular color -- white
        // ka,  kd,   ks,   ke
        0.0f, 0.8f, 0.0f, 0.0f
    );
    plane_mat = new SolidMaterial(
        vec3( 0.8f, 0.8f, 0.8f ), // color
        //kr,   kd
        0.0f, 0.0f
    );
    rect = new Rectangle(
        pt_h, pt_g, pt_c, pt_d,
        plane_imodel,
        plane_mat
    );
    world->add_object( rect );

    //
    // back wall
    //

    plane_imodel = new Phong(
        vec3( 1.0f, 1.0f, 1.0f ), // specular color -- white
        // ka,  kd,   ks,   ke
        0.0f, 0.8f, 0.0f, 0.0f
    );
    plane_mat = new SolidMaterial(
        vec3( 0.8f, 0.8f, 0.8f ), // color
        //kr,   kd
        0.0f, 0.0f
    );
    rect = new Rectangle(
        pt_e, pt_f, pt_g, pt_h,
        plane_imodel,
        plane_mat
    );
    world->add_object(rect);

    //
    // ceiling
    //

    plane_imodel = new Phong(
        vec3( 1.0f, 1.0f, 1.0f ), // specular color -- white
        // ka,  kd,   ks,   ke
        0.0f, 0.8f, 0.0f, 0.0f
    );
    plane_mat = new SolidMaterial(
        vec3( 0.8f, 0.8f, 0.8f ), // color
        //kr,   kd
        0.0f, 0.0f
    );
    rect = new Rectangle(
        pt_a, pt_b, pt_f, pt_e,
        plane_imodel,
        plane_mat
    );
    world->add_object(rect);

    //
    // left wall
    //

    plane_imodel = new Phong(
        vec3( 1.0f, 1.0f, 1.0f ), // specular color -- white
        // ka,  kd,   ks,   ke
        0.0f, 0.8f, 0.0f, 0.0f
    );
    // TODO refine this colour
    plane_mat = new SolidMaterial(
        vec3( 0.8f, 0.1f, 0.1f ), // color
        //kr,   kd
        0.0f, 0.0f
    );
    rect = new Rectangle(
        pt_a, pt_e, pt_h, pt_d,
        plane_imodel,
        plane_mat
    );
    world->add_object(rect);

    //
    // right wall
    //

    plane_imodel = new Phong(
        vec3( 1.0f, 1.0f, 1.0f ), // specular color -- white
        // ka,  kd,   ks,   ke
        0.0f, 0.8f, 0.0f, 0.0f
    );
    // TODO refine this colour
    plane_mat = new SolidMaterial(
        vec3( 0.3f, 0.1f, 0.8f ), // color
        //kr,   kd
        0.0f, 0.0f
    );
    rect = new Rectangle(
        pt_f, pt_b, pt_c, pt_g,
        plane_imodel,
        plane_mat
    );
    world->add_object(rect);

    // =======================
    //         SPHERES
    // =======================

    // float sphere_trans = 1.0f;
    // float sphere_r = 0.5f * sphere_trans;
    //
    // //
    // // REFLECTIVE
    // //
    //
    // vec3 sphere1_p = vec3( 0.0f - 1.5 * sphere_r, 0.0f + sphere_r, 0.0f );
    // Phong* sphere1_imodel = new Phong(
    //  vec3( 0.0f, 0.0f, 0.0f ), // color
    //  //ka,   kd,   ks,   ke
    //  0.1f, 0.5f, 0.1f, 20.0f
    // );
    // SolidMaterial* sphere1_mat = new SolidMaterial(
    //  vec3(0.0f, 0.0f, 0.0f), // color
    //  //kr,   kd
    //  1.0f, 0.0f
    // );
    //
    // //
    // // TRANSMISSIVE
    // //
    //
    // vec3 sphere2_p = vec3(
    //     0.0f - 1.5 * sphere_r, // x
    //     0.0f + sphere_r,       // y
    //     0.0f
    // );
    // Phong* sphere2_imodel = new Phong(
    //  vec3( 0.0f, 0.0f, 0.0f ), // color
    //  //ka,   kd,   ks,   ke
    //  0.1f, 0.5f, 0.1f, 20.0f
    // );
    // SolidMaterial* sphere2_mat = new SolidMaterial(
    //  vec3(0.0f, 0.0f, 0.0f), // color
    //  //kr,   kd,   ir
    //  0.0f, 1.0f, 0.7f
    // );
    //
    // Sphere* sphere1 = new Sphere( &sphere1_p, sphere_r, sphere1_imodel, sphere1_mat );
    // Sphere* sphere2 = new Sphere( &sphere2_p, sphere_r, sphere2_imodel, sphere2_mat );
    //
    // world->add_object( sphere1 );
    // world->add_object( sphere2 );

    // =======================
    //          LIGHT
    // =======================

    vec3* light_col = new vec3( 1.0f );
    vec3* light_pos = new vec3( 0.0f ); //, cube_r, 0.0f );

    Light light = {
        light_col, light_pos
    };

    world->add_light( &light );

    // main camera
    Camera* cam = new Camera(
        world,
        vec3( 0.0f, 0.0f, -cube_w ), // pos
        vec3( 0.0f, 0.0f, 1.0f ), // lookat
        vec3( 0.0f, 1.0f, 0.0f ), // up
        { 1.92f, 1.08f, 0.8f } // P_Plane
    );

    png::image<png::rgb_pixel> negative( I_WIDTH, I_HEIGHT );

    std::clock_t start;
    double duration;

    cam->set_scene();

    start = std::clock();

    cam->render( &negative );
    photo_print( negative, "out/test.png" );

    duration = start - std::clock();

    std::cout << "render took " << duration << '\n';

    delete cam;
}

int main( void ) {

    // initialize the stuff we need for our rendering
    cornell_box();

    return 0;
}
