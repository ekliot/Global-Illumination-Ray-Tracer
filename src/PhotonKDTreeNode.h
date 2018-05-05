#ifndef _PhotonKDTreeNode_H
#define _PhotonKDTreeNode_H

#include <queue>
#include <vector>

#include "AABB.h"
#include "Photon.h"
#include "Ray.h"

class PhotonKDTreeNode {
    const int MAX_DEPTH = 25;

  public:
    AABB* aabb;
    PhotonKDTreeNode* left;
    PhotonKDTreeNode* right;
    int axis;

    std::vector<Photon*> photons;

    PhotonKDTreeNode( std::vector<Photon*> _photons, AABB* _aabb, int counter );
    std::priority_queue<Photon> getPhotons( vec3 point );
    ~PhotonKDTreeNode();

    int set_aabbs( AABB* aabb, AABB* left, AABB* right, float dx, float dy,
                   float dz );
};
#endif
