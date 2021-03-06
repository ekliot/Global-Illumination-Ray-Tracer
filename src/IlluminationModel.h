/**
 *
 */

#ifndef _ILLUIMINATIONMODEL_H_
#define _ILLUIMINATIONMODEL_H_

#include <glm/vec3.hpp>

#include "IntersectData.h"

using glm::vec3;

class IlluminationModel {
  public:
    IlluminationModel();
    // IlluminationModel( const IlluminationModel& model );
    virtual ~IlluminationModel() = 0;
    // virtual IlluminationModel& operator=( const IlluminationModel& model ) =
    // 0;

    virtual vec3 intersect( IntersectData idata ) = 0;

    virtual vec3 get_diffuse( vec3 l_col, vec3 obj_col, vec3 norm,
                              vec3 dir )                               = 0;
    virtual vec3 get_specular( vec3 l_col, vec3 ref_dir, vec3 in_dir ) = 0;

    virtual float get_kd() = 0;
    virtual float get_ks() = 0;
};

#endif
