#ifndef _KDTreeNode_H
#define _KDTreeNode_H

#include "AABB.h"
#include "Ray.h"
#include "Object.h"

class KDTreeNode {



public:
    KDTreeNode* left;
    KDTreeNode* right;

    std::vector<Object*>* objects;
    AABB* aabb;
    KDTreeNode(std::vector<Object*> _objects, AABB* _aabb, int count);

};
#endif
