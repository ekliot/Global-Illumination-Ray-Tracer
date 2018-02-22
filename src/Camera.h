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

#include "png++/png.hpp"

#include "PPlane.h"
#include "World.h"

using namespace png;
using glm::vec3;

class Camera {

    World* world;

    vec3 pos;
    vec3 lookat;
    vec3 up;

    PPlane plane;

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
    Camera( World *w, vec3 p, vec3 l, vec3 u, PPlane pp );

    /**
     * Destructor
     */
    ~Camera( void );

    void render( image<rgb_pixel> *negative );

};

#endif
