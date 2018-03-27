/**
 *
 */

#include <iostream>

#include "glm/gtx/string_cast.hpp"

using namespace std;
using glm::vec3;
using glm::vec4;

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
