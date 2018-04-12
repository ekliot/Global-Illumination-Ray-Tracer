/**
 *
 */

#include <iostream>
#include "glm/gtx/string_cast.hpp"
#include <vector>
#include <math.h>

#include <glm/glm.hpp>

#include "Ray.h"

using namespace glm;

Ray::Ray( vec3* ori, vec3* dir ) {
    origin = new vec3( ori->x, ori->y, ori->z );
    direction = new vec3( glm::normalize( *dir ) );
}

Ray::~Ray() {
    delete origin;
    delete direction;
}

Ray* Ray::reflect( vec3* normal ){
    vec3 _ori = vec3( origin->x, origin->y, origin->z );
    vec3 _dir = glm::reflect( *(direction), *(normal) );

    Ray* reflected = new Ray( &_ori, &_dir );

    return reflected;
}
void Ray::print(){
    std::cout << "origin ->  " << glm::to_string(*origin) << '\n';
    std::cout << "direction ->  " << glm::to_string(*direction) << '\n';

}
