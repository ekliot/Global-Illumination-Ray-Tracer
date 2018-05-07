/**
 *
 */

#ifndef _PHOTON_H_
#define _PHOTON_H_

#include <glm/detail/func_vector_relational.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <queue>
#include <vector>

#include "glm/gtx/string_cast.hpp"

using namespace glm;
using namespace std;

namespace photon {
    typedef struct st_Photon {
        vec3 position;
        vec3 power;
        vec3 dir;
        vec3 src;
        bool shadow    = false;
        float distance = 0.0f;  // used for min heap
    } Photon;

    struct compare {
        bool operator()( const st_Photon* a, const st_Photon* b ) {
            // TODO
            return a->distance > b->distance;
        }
    };

    typedef priority_queue<Photon*, vector<Photon*>, compare> PhotonHeap;
}  // namespace photon

#endif
