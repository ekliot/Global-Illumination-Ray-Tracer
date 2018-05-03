
#include "Material.h"

/**************\
 PUBLIC MEMBERS
\**************/

Material::Material(float _kr, float _kd, float _ir ):kr(_kr), kd(_kd), ir(_ir){}
Material::~Material(){}

float Material::get_kd(){
    return kd;
}

float Material::get_kr(){
    return kr;
}

float Material::get_ir(){
    return ir;
}
