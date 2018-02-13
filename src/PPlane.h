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
    float w; // width in world coordinates
    float h; // height in world coordinates
    float foc_l; // focal length
} PPlane;

#endif
