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
    uint_t w; // width
    uint_t h; // height
    float foc_l; // focal length
} PPlane;

#endif
