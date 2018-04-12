
#include "Material.h"

/**************\
 PUBLIC MEMBERS
\**************/

Material::Material(float _kr, float _kd):kr(_kr), kd(_kd){}
Material::~Material(){}

float Material::get_kd(){
    return kd;
}

float Material::get_kr(){
    return kr;
}
