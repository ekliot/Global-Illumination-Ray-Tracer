/**
 *
 */

#ifndef _KDTreeNode_H
#define _KDTreeNode_H

#include "AABB.h"
#include "Object.h"
#include "Ray.h"

class KDTreeNode {
<<<<<<< Updated upstream
  public:
    KDTreeNode* left;
    KDTreeNode* right;

    std::vector<Object*>* objects;
    AABB* aabb;
    KDTreeNode( std::vector<Object*> _objects, AABB* _aabb, int count );
=======
    const int MAX_DEPTH = 25;

  public:
    KDTreeNode *left;
    KDTreeNode *right;

    std::vector<Object *> objects;
    AABB *aabb;
    KDTreeNode( std::vector<Object *> _objects, AABB *_aabb, int count );
>>>>>>> Stashed changes
};

#endif
