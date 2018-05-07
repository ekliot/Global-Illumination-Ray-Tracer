/**
 *
 */

#include "AABB.h"

#include <glm/glm.hpp>

#include "glm/gtx/string_cast.hpp"

using namespace glm;

AABB::AABB( float max_x, float max_y, float max_z, float min_x, float min_y,
            float min_z ) {
    max = vec3( max_x, max_y, max_z );
    min = vec3( min_x, min_y, min_z );
}

AABB::AABB( vec3 _max, vec3 _min ) : max( vec3( _max ) ), min( vec3( _min ) ) {}

AABB::AABB( AABB* a, AABB* b ) {
    max = vec3( std::max( a->max.x, b->max.x ), std::max( a->max.y, b->max.y ),
                std::max( a->max.z, b->max.z ) );
    min = vec3( std::min( a->min.x, b->min.x ), std::min( a->min.y, b->min.y ),
                std::min( a->min.z, b->min.z ) );
}

bool AABB::intersect_aabb( AABB* aabb ) {
    // if (Aaxis.min > Baxis.max)
    // or
    //    (Baxis.min > Aaxis.max) return FALSE

    if ( min.x > aabb->max.x || aabb->min.x > max.x || min.y > aabb->max.y ||
         aabb->min.y > max.y || min.z > aabb->max.z || aabb->min.z > max.z )
        return false;

    return true;
}

bool AABB::intersect_point( vec3 point ) {
    // std::cout << glm::to_string( point ) << '\n' << std::endl;
    // std::cout << glm::to_string( min ) << '\n' << std::endl;
    // std::cout << glm::to_string( max ) << '\n' << std::endl;
    if ( min.x < point.x && max.x > point.x && min.y < point.y &&
         max.y > point.y && min.z < point.z && max.z > point.z ) {
        return true;
    }
    return false;
}

float AABB::intersect_ray( Ray* ray ) {
    vec3 dirfrac;
    // r.dir is unit direction vector of ray
    dirfrac.x = 1.0f / ray->direction->x;
    dirfrac.y = 1.0f / ray->direction->y;
    dirfrac.z = 1.0f / ray->direction->z;

    // lb is the corner of AABB with minimal coordinates - left bottom, rt is
    // maximal corner r.org is origin of ray
    float t1 = ( min.x - ray->origin->x ) * dirfrac.x;
    float t2 = ( max.x - ray->origin->x ) * dirfrac.x;
    float t3 = ( min.y - ray->origin->y ) * dirfrac.y;
    float t4 = ( max.y - ray->origin->y ) * dirfrac.y;
    float t5 = ( min.z - ray->origin->z ) * dirfrac.z;
    float t6 = ( max.z - ray->origin->z ) * dirfrac.z;

    float tmin = std::max( std::max( std::min( t1, t2 ), std::min( t3, t4 ) ),
                           std::min( t5, t6 ) );
    float tmax = std::min( std::min( std::max( t1, t2 ), std::max( t3, t4 ) ),
                           std::max( t5, t6 ) );

    // if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is
    // behind us
    // if tmin > tmax, ray doesn't intersect AABB
    if ( tmax >= 0 && tmin > tmax ) {
        return INT_MAX;
    }

    return tmax;
}

vec3 AABB::get_max() { return vec3( max ); }

vec3 AABB::get_min() { return vec3( min ); }

void AABB::print() {
    std::cout << "AABB:" << '\n';
    std::cout << "\tmax: " << glm::to_string( max ) << '\n';
    std::cout << "\tmin: " << glm::to_string( min ) << '\n';
}
