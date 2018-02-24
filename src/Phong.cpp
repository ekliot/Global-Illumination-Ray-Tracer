/**
 *
 */

#include <cmath>
#include <iostream>
#include <stdexcept>

#include <glm/glm.hpp>
#include "glm/gtx/string_cast.hpp"

#include "Phong.h"
#include "Ray.h"

using glm::vec3;

Phong::Phong( vec3 _col_o, vec3 _col_s, float _ka, float _kd, float _ks, float _ke ) :
         color_obj(_col_o), color_spec( _col_s), ka(_ka), kd(_kd), ks(_ks), ke(_ke) {
    if ( _kd + _ks >= 1 ) {
        std::string except = "Phong ks + kd must be < 1 (provided " + std::to_string(_ks) + " and " + std::to_string(_kd) + ")";
        throw std::invalid_argument( except );
    }
}

vec3 Phong::intersect( IntersectData idata ) {
    vec3 ambient = *(idata.ambient) * ka;

    vec3 diffuse  = vec3( 0.0f );
    vec3 specular = vec3( 0.0f );

    for( Light* l : idata.lights ) {
        vec3 l_dir = *(l->position) - *(idata.position);

        Ray* source  = new Ray( l->position, &l_dir );
        Ray* reflect = source->reflect( idata.normal );

        // std::cout << "diffuse += " << glm::to_string( *(l->color) ) << " * " << glm::to_string( color_obj ) << " * ( " << dot( *(idata.normal), *(source->direction) ) << " := " << glm::to_string( *(idata.normal) ) << " dot " << glm::to_string( *(source->direction) ) << " )" << '\n';

        diffuse = diffuse + *(l->color) * color_obj * dot( *(idata.normal), *(source->direction) );
        specular = specular + *(l->color) * color_spec * pow( dot( *(reflect->direction), *(idata.incoming) ), ke );
    }

    diffuse  = kd * diffuse;
    specular = ks * specular;

    vec3 light = ambient + diffuse + specular;

    if ( light.x > 1.0f || light.y > 1.0f || light.z > 1.0f ) {
        std::cout << "light // " << glm::to_string( light ) << '\n';
        std::cout << "ambient // " << glm::to_string( ambient ) << '\n';
        std::cout << "diffuse // " << glm::to_string( diffuse ) << '\n';
        std::cout << "specular // " << glm::to_string( specular ) << '\n';
    }

    return light;
}
