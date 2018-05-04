/**
 *
 */

#include <iostream>

#include "glm/gtx/string_cast.hpp"

using glm::vec3;
using glm::vec4;
using namespace std;

#include "Rectangle.h"

/**************\
 PUBLIC MEMBERS
\**************/

Rectangle::Rectangle( vec3* _ul, vec3* _ur, vec3* _lr, vec3* _ll, IlluminationModel* _imodel, Material* _mat ) : \
        Object(_imodel,_mat), ul(_ul), ur(_ur), lr(_lr), ll(_ll) {

    // TODO these should COPY imodel and material
    tri1 = new Triangle( ll, lr, ul, imodel, material );
    tri2 = new Triangle( ul, ur, lr, imodel, material );

}

Rectangle::~Rectangle() {
    // TODO see constructor for why these are commented out
    // delete tri1;
    // delete tri2;
}

void Rectangle::transform( mat4 mat ) {
    vec4 _ul = vec4( ul->x, ul->y, ul->z, 1 );
    _ul = mat * _ul;
    *ul = convert( &_ul );

    vec4 _ur = vec4( ur->x, ur->y, ur->z, 1 );
    _ur = mat * _ur;
    *ur = convert( &_ur );

    vec4 _lr = vec4( lr->x, lr->y, lr->z, 1 );
    _lr = mat * _lr;
    *lr = convert( &_lr );

    vec4 _ll = vec4( ll->x, ll->y, ll->z, 1 );
    _ll = mat * _ll;
    *ll = convert( &_ll );

    tri1->transform(mat);
    tri2->transform(mat);
}

float Rectangle::intersection( Ray* ray ) {
    float temp = tri2->intersection( ray );
    if ( temp == INT_MAX ) {
        return tri1->intersection( ray );
    }
    return temp;
}

vec3 Rectangle::get_normal( Ray* ray, float distance ) {
    return tri1->get_normal( ray, distance );
}

AABB* Rectangle::getAABB()
{
    vec3 vec_max = vec3(ul->x, ul->y, ul->z);
    vec3 vec_min = vec3(ul->x, ul->y, ul->z);

    vec_max = maxVecHelper(vec_max, *ur);
    vec_max = maxVecHelper(vec_max,*ll);
    vec_max = maxVecHelper(vec_max,*lr);

    vec_min = minVecHelper(vec_min, *ur);
    vec_min = minVecHelper(vec_min,*ll);
    vec_min = minVecHelper(vec_min,*lr);


    return new AABB(vec_max.x, vec_max.y, vec_max.z, vec_min.x, vec_min.y, vec_min.z);
}
// TODO actually implement world-to-obj conversion
vec3 Rectangle::world_to_obj_space( vec3 point ) {
    return point;
}

// TODO actually implement uv coords for real
// HACK this does not bother converting to object coordinates!!!
vec2 Rectangle::get_uv( vec3 point ) {
    float u = 0.0f;
    float v = 0.0f;

    if ( ul->y == lr->y && ur->y == ll->y ) {
        float x_min = std::min( ul->x, std::min( ur->x, std::min( lr->x, ll->x ) ) );
        float z_min = std::min( ul->z, std::min( ur->z, std::min( lr->z, ll->z ) ) );

        float x_max = std::max( ul->x, std::max( ur->x, std::max( lr->x, ll->x ) ) );
        float z_max = std::max( ul->z, std::max( ur->z, std::max( lr->z, ll->z ) ) );

        float w = x_max - x_min;
        float l = z_max - z_min;

        u = ( x_max - point.x ) / w;
        v = ( z_max - point.z ) / l;
    }

    return vec2( u, v );
}
