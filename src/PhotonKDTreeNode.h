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
    const size_t MAX_DEPTH = 30;

  public:
    AABB* aabb;
    PhotonKDTreeNode* left  = NULL;
    PhotonKDTreeNode* right = NULL;
    int axis;

    std::vector<Photon*> photons;

    PhotonKDTreeNode( std::vector<Photon*> _photons, AABB* _aabb,
                      size_t depth );
    ~PhotonKDTreeNode();

    void get_photons_near_pt( vector<Photon*>* heap, vec3 position,
                              float range );
    void get_n_photons_near_pt( vector<Photon*>* heap, vec3 position,
                                size_t size, float* range );
};
#endif
