/**
 * World.cpp
 *
 * Author: ekliot
 */
#include "stdafx.h"
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <glm\matrix.hpp>
#endif

#include <vector>
#include <glm/vec3.hpp>
//#include <glm/mat4.hpp>

#include "Ray.h"
#include "Object.h"

World::World() {}

World::~World() {}

void add( Object obj ) {
    //
}

void transform_all_to_ccoord( glm::mat4 tmat ) {
    //
}

glm::vec3 get_intersect( Ray r ) {
    //
}
