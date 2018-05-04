#ifndef _PhotonKDTreeNode_H
#define _PhotonKDTreeNode_H

#include "AABB.h"
#include "Ray.h"
#include "Photon.h"
#include <vector>
#include <queue>


class PhotonKDTreeNode {



public:
    PhotonKDTreeNode* left;
    PhotonKDTreeNode* right;
    int axis;

    std::vector<Photon>* photons;
    AABB* aabb;
    
    PhotonKDTreeNode(std::vector<Photon> _photons, AABB* _aabb, int counter);
    std::priority_queue<Photon> getPhotons(vec3 point);


};
#endif
