/**
 *
 */

#ifndef _ILLUIMINATIONMODEL_H_
#define _ILLUIMINATIONMODEL_H_

#include <glm/vec3.hpp>

#include "IntersectData.h"

using namespace glm;

class IlluminationModel {
public:
    IlluminationModel();
    virtual ~IlluminationModel() = 0;
    virtual vec3 intersect( IntersectData idata ) = 0;
};

#endif
