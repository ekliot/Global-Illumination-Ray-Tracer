/**
 *
 */

#include "Phong.h"

#include <cmath>
#include <glm/glm.hpp>
#include <iostream>
#include <stdexcept>

#include "Ray.h"
#include "glm/gtx/string_cast.hpp"

using glm::vec3;

Phong::Phong( vec3 _col_s, float _ka, float _kd, float _ks, float _ke )
    : spec_col( _col_s ), ka( _ka ), kd( _kd ), ks( _ks ), ke( _ke ) {
    if ( _kd + _ks >= 1 ) {
        std::string except = "Phong ks + kd must be < 1 (provided " +
                             std::to_string( _ks ) + " and " +
                             std::to_string( _kd ) + ")";
        throw std::invalid_argument( except );
    }
}

Phong::~Phong() {}

vec3 Phong::intersect( IntersectData idata ) {
    vec3 ambient = *( idata.ambient ) * ka;

    // std::cout << "idata.incoming // " << glm::to_string( *(idata.incoming) )
    // << '\n'; std::cout << "idata.normal   // " << glm::to_string(
    // *(idata.normal) ) << '\n';

    vec3 diffuse  = vec3( 0.0f );
    vec3 specular = vec3( 0.0f );

    for ( Light* l : idata.lights ) {
        // if ( *idata.obj_color == vec3( 0.0f, 1.0f, 0.0f ) ) {
        //     std::cout << "light {" << '\n';
        //     std::cout << "  col: " << glm::to_string( *l.color ) << '\n';
        //     std::cout << "  pos: " << glm::to_string( *l.position ) << '\n';
        //     std::cout << "}" << '\n';
        // }

        vec3 l_pos = l->get_pos();
        vec3 l_col = l->get_color();
        vec3 l_dir = *( idata.position ) - l_pos;

        Ray* source  = new Ray( &l_pos, &l_dir );
        Ray* reflect = source->reflect( idata.normal );

        diffuse = diffuse + get_diffuse( l_col, *idata.obj_color, *idata.normal,
                                         *source->direction );
        specular = specular +
                   get_specular( l_col, *reflect->direction, *idata.incoming );

        delete source;
        delete reflect;
    }

    diffuse  = kd * diffuse;
    specular = ks * specular;

    vec3 light = ambient + diffuse + specular;

    // if ( light.x < 0.5f && light.y < 0.5f && light.z < 0.5f ) {
    // if ( *idata.obj_color == vec3(0.0f,1.0f,0.0f) ) {
    //     std::cout << '\n';
    //     std::cout << "light        // " << glm::to_string( light ) << '\n';
    //     std::cout << "  + ambient  // " << glm::to_string( ambient ) << '\n';
    //     std::cout << "  + diffuse  // " << glm::to_string( diffuse ) << '\n';
    //     std::cout << "  + specular // " << glm::to_string( specular ) <<
    //     '\n'; std::cout << "         obj // " << glm::to_string(
    //     *idata.obj_color ) << '\n';
    // }

    return light;
}

vec3 Phong::get_diffuse( vec3 l_col, vec3 obj_col, vec3 norm, vec3 dir ) {
    return l_col * obj_col * abs( dot( norm, dir ) );
}

vec3 Phong::get_specular( vec3 l_col, vec3 ref_dir, vec3 in_dir ) {
    return l_col * spec_col * pow( dot( ref_dir, in_dir ), ke );
}

float Phong::get_kd() { return kd; }

float Phong::get_ks() { return ks; }
