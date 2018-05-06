/**
 * Camera.h
 *
 * Camera object for rendering a world using ray tracing
 *
 * Author: ekliot
 */

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <stdint.h>

#include <glm/vec3.hpp>

#include "PPlane.h"
#include "World.h"
#include "png++/png.hpp"

using namespace png;
using glm::mat4;
using glm::vec3;

class Camera {
    World* world;
    mat4 transform_mat;
    PPlane plane;
    bool is_set = false;

  public:
    /**
     * Constructor
     *
     * @param w  :: World* :: pointer to the world the camera is rendering
     * @param p  :: vec3   :: the position of the camera
     * @param l  :: vec3   :: the lookat vector of the camera
     * @param u  :: vec3   :: the up vector of the camera
     * @param pp :: PPlane :: the projection plane for the camera
     */
    Camera( World* w, vec3 p, vec3 l, vec3 u, PPlane pp );

    /**
     * Destructor
     */
    ~Camera( void );

    void set_scene();
    void break_scene();

    /**
     * ss_rate  //  0   1   2   3   4   5   6
     * SS level // x0  x2  x4  x6  x9 x12 x16
     */
    void render( image<rgb_pixel>* negative, uint ss_rate = 0 );
};

#endif
