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
    // virtual IlluminationModel& operator=( const IlluminationModel& model ) = 0;

    virtual vec3 intersect( IntersectData idata ) = 0;
};

#endif
