#include "KDTreeNode.h"
#include <glm/glm.hpp>

KDTreeNode::KDTreeNode(std::vector<Object*> _objects, AABB* _aabb, int count) :
objects(_objects), aabb(_aabb)
 {
     if(objects.size() > 2)
     {
         std::vector<Object*> firstList;
         std::vector<Object*> secondList;

         AABB* firstAABB = NULL;
         AABB* secondAABB = NULL;

         if(count % 3 == 0)
         {
             float xLength = (aabb->getMax().x - aabb->getMin().x)/2;

             firstAABB = new AABB(aabb->getMax().x - xLength, aabb->getMax().y, aabb->getMax().z,
             aabb->getMin().x, aabb->getMin().y, aabb->getMin().z);
             secondAABB = new AABB(aabb->getMax().x, aabb->getMax().y, aabb->getMax().z,
             aabb->getMin().x + xLength, aabb->getMin().y, aabb->getMin().z);
         }
         else if(count % 3 == 1)
         {
             float yLength = (aabb->getMax().y - aabb->getMin().y)/2;

             firstAABB = new AABB(aabb->getMax().x, aabb->getMax().y - yLength, aabb->getMax().z,
             aabb->getMin().x, aabb->getMin().y, aabb->getMin().z);
             secondAABB = new AABB(aabb->getMax().x, aabb->getMax().y, aabb->getMax().z,
             aabb->getMin().x, aabb->getMin().y + yLength, aabb->getMin().z);
         }
         else
         {
             float zLength = (aabb->getMax().z - aabb->getMin().z)/2;

             firstAABB = new AABB(aabb->getMax().x, aabb->getMax().y, aabb->getMax().z - zLength,
             aabb->getMin().x, aabb->getMin().y, aabb->getMin().z);
             secondAABB = new AABB(aabb->getMax().x, aabb->getMax().y, aabb->getMax().z,
             aabb->getMin().x, aabb->getMin().y, aabb->getMin().z + zLength);
         }

        for (const auto& object : objects)
        {
            AABB* currentAABB = object->getAABB();
            if(currentAABB->intersectAABB(firstAABB))
            {
                firstList.push_back(object);
            }
            if(currentAABB->intersectAABB(secondAABB))
            {
                secondList.push_back(object);
            }
        }

        left = new KDTreeNode(firstList, firstAABB, ++count);
        right = new KDTreeNode(secondList, secondAABB, ++count);

     }
     else
     {
         // we gucci fam
     }
 }
