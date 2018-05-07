/**
 *
 */

#ifndef _PHONG_H_
#define _PHONG_H_

#include <glm/vec3.hpp>

#include "IlluminationModel.h"

class Phong : public IlluminationModel {
    vec3 spec_col;
    float ka;
    float kd;
    float ks;
    float ke;

  public:
    Phong( vec3 _col_s, float _ka, float _kd, float _ks, float _ke );
    ~Phong();

    vec3 intersect( IntersectData idata );

    vec3 get_diffuse( vec3 l_col, vec3 obj_col, vec3 norm, vec3 dir );
    vec3 get_specular( vec3 l_col, vec3 spec, vec3 ref_dir, vec3 in_dir,
                       float ke );

    float get_kd();
    float get_ks();
};

#endif
