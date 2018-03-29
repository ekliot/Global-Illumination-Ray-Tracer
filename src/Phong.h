/**
 *
 */

#ifndef _PHONG_H_
#define _PHONG_H_

#include <glm/vec3.hpp>

#include "IlluminationModel.h"

class Phong : public IlluminationModel {
    vec3 color_spec;
    float ka;
    float kd;
    float ks;
    float ke;

public:
    Phong( vec3 _col_s, float _ka, float _kd, float _ks, float _ke );
    ~Phong();

    vec3 intersect( IntersectData idata ); 
};

#endif
