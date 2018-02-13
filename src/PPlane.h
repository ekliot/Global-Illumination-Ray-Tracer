/**
 * PPlane.h
 *
 * Representation of a projection plane for ray tracing
 *
 * Author: ekliot
 */

#ifndef _PPLANE_H_
#define _PPLANE_H_

#include <stdint.h>

typedef struct st_PPlane {
    int w; // width
    int h; // height
    float foc_l; // focal length
    float ratio; // pixel:world_unit ratio
} PPlane;

#endif
