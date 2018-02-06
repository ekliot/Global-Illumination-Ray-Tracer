/**
 * Camera.h
 *
 * Camera object for rendering a world using ray tracing
 *
 * Author: ekliot
 */


#ifndef _CAMERA_H_
#define _CAMERA_H_

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#include <stdint.h>

#include <glm/vec3.hpp>
#include "PPlane.h"
#include "World.h"

using namespace glm;

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
     * @param up :: vec3   :: the up vector of the camera
     * @param pp :: PPlane :: the projection plane for the camera
     */
    Camera( World *w, vec3 p, vec3 l, vec3 up, PPlane pp );

    /**
     * Destructor
     */
    ~Camera( void );

    void render( uint8_t *px_buf );

};

#endif
