/**
 *
 */

#ifndef _PhotonKDTreeNode_H
#define _PhotonKDTreeNode_H

#include <queue>
#include <vector>

#include "AABB.h"
#include "Photon.h"
#include "Ray.h"

using namespace std;
using namespace photon;
using glm::vec3;

class PhotonKDTreeNode {
    const int MAX_DEPTH = 25;

  public:
    AABB* aabb;
    PhotonKDTreeNode* left;
    PhotonKDTreeNode* right;
    int axis;

    std::vector<Photon*> photons;

    PhotonKDTreeNode( std::vector<Photon*> _photons );
    PhotonKDTreeNode( std::vector<Photon*> _photons, AABB* _aabb, int depth );
    ~PhotonKDTreeNode();

    int set_aabbs( AABB* left, AABB* right, float dx, float dy, float dz );

    void get_photons_near_pt( PhotonHeap* heap, vec3 position, float range );
    void get_n_photons_near_pt( PhotonHeap* heap, vec3 position, float* range, size_t size );

};
#endif
