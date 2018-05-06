/**
 *
 */

#ifndef _PHOTON_H_
#define _PHOTON_H_

#include <glm/detail/func_vector_relational.hpp>
#include <glm/vec3.hpp>
#include <queue>
#include <vector>

using namespace glm;
using namespace std;

namespace photon {
    typedef struct st_Photon {
        vec3 position;
        vec3 power;
        vec3 dir;
        float distance;  // used for min heap
        // short flag;
    } Photon;

    struct compare {
        bool operator()( const st_Photon* a, const st_Photon* b ) {
            // TODO
            return false;
        }
    };

    typedef priority_queue<Photon*, vector<Photon*>, compare> PhotonHeap;
}  // namespace photon

#endif
