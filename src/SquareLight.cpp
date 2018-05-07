/**
 *
 */
 #define PI 3.14159265

#include "SquareLight.h"

#include <stdlib.h>

#include "Phong.h"
#include "SolidMaterial.h"

SquareLight::SquareLight( vec3 _pos, vec3 _norm, vec3 _col, int _photons,
                          float _w, float _l, float _max )
    : Light( _pos, _col, _photons ), normal( _norm ), max_angle( _max ) {
    Phong* imod        = new Phong( vec3( 1.0f ), 0.0f, 0.0f, 0.0f, 0.0f );
    SolidMaterial* mat = new SolidMaterial( _col );
    // make a rectangle
    // HACK this just makes a rectangle pointing straight down
    vec3 pt_a = vec3( pos.x - _w / 2, pos.y, pos.z + _l / 2 );
    vec3 pt_b = vec3( pos.x + _w / 2, pos.y, pos.z + _l / 2 );
    vec3 pt_c = vec3( pos.x + _w / 2, pos.y, pos.z - _l / 2 );
    vec3 pt_d = vec3( pos.x - _w / 2, pos.y, pos.z - _l / 2 );
    rect      = new Rectangle( pt_a, pt_b, pt_c, pt_d, imod, mat );
}

SquareLight::~SquareLight() {
    // if ( rect != nullptr ) {
    //     delete rect;
    //     rect = nullptr;
    // }
}

std::vector<Photon*> SquareLight::emit_photons() {
    // for n times emit photons with 1/n power
    return std::vector<Photon*>();
}

Photon* SquareLight::emit( float power ) {
    vec3 origin = gen_emit_ori();
    vec3 dir    = gen_emit_dir();

    Photon* p   = new Photon();
    p->position = origin;
    p->power    = color * power;
    p->dir      = dir;
    p->distance = 0.0f;

    return p;
}

vec3 SquareLight::gen_emit_ori() {
    // HACK doesn't care about Y-axis
    float _x = static_cast<float>( rand() ) / static_cast<float>( RAND_MAX );
    float _z = static_cast<float>( rand() ) / static_cast<float>( RAND_MAX );

    AABB* aabb = rect->get_aabb();

    vec3 min = aabb->get_min();
    vec3 max = aabb->get_max();

    float dx = ( max.x - min.x ) * _x;
    float dz = ( max.z - min.z ) * _z;

    delete aabb;

    return vec3( min.x + dx, min.y, min.z + dz );
}

vec3 SquareLight::gen_emit_dir() {

    float random_x = (PI/2) - static_cast <float> (rand()) /
        static_cast <float> (RAND_MAX) * (PI);
    float random_x_cos = (1 - cos(random_x)) * max_angle;
    vec3 x_vec;
    if(random_x < 0)
    {
        x_vec = vec3(random_x_cos,0,0);
    }
    else
    {
        x_vec = vec3(- random_x_cos,0,0);
    }

    float random_z = (PI/2) - static_cast <float> (rand()) /
        static_cast <float> (RAND_MAX) * (PI);
    float random_z_cos = (1 - cos(random_z)) * max_angle;
    vec3 z_vec;
    if(random_z < 0)
    {
        z_vec = vec3(0,0,random_z_cos);
    }
    else
    {
        z_vec = vec3(0,0,-random_z_cos);
    }

    return normalize(normal + z_vec + x_vec);





    // HACK implement a cosine distribution b/w -max_angle and max_angle
    //return normal;
}

float SquareLight::intersection( Ray* ray ) {
    return rect->intersection( ray );
}

void SquareLight::transform( mat4 tmat ) {
    //rect->transform( tmat );

    vec4 _pos    = vec4( pos, 1 );
    vec4 new_pos = tmat * _pos;
    pos          = rect->convert( new_pos );
}

Object* SquareLight::get_obj() { return rect; }
