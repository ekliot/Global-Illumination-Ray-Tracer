/**
 * World.cpp
 *
 * Author: ekliot
 */

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4.hpp>

#include "Ray.h"
#include "Object.h"

World::World() {}

World::~World() {}

void World::add( Object obj ) {
    //
}

void World::transform_all_to_ccoord( mat4 tmat ) {
    //
}

vec3 World::get_intersect( Ray r ) {
    //
}
