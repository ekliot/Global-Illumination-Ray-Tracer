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
    // std::cout << "reflect( " << glm::to_string( *normal ) << " )" << '\n';
    vec3 _ori =  vec3( origin->x, origin->y, origin->z );
    vec3 _dir = glm::reflect( *(direction), *(normal) );
    // std::cout << "  _ori // " << glm::to_string( _ori ) << '\n';
    // std::cout << "  _dir // " << glm::to_string( _dir ) << '\n';

    Ray* reflected = new Ray( &_ori, &_dir );
    // std::cout << "  reflected.dir // " << glm::to_string( *(reflected->direction) ) << '\n';
    // std::cout << "  reflected.ori // " << glm::to_string( *(reflected->origin) ) << '\n';

    return reflected;
}
