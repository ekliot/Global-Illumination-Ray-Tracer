#include "KDTreeNode.h"
#include <glm/glm.hpp>
#include <iostream>
using namespace std;

KDTreeNode::KDTreeNode(std::vector<Object*> _objects, AABB* _aabb, int count) :
aabb(_aabb)
{
    objects = new std::vector<Object*>();
    for(Object *  o : _objects)
    {
        objects->push_back(o);
    }

     if(objects->size() >= 2 && count  < 25)
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

        // std::cout << "firstAABB" << '\n';
        // firstAABB->print();
        // std::cout << "secondAABB" << '\n';
        // secondAABB->print();
        for (const auto& object : *objects)
        {
            AABB* currentAABB = object->getAABB();
            // std::cout << "currentAABB" << '\n';
            // currentAABB->print();
            if(currentAABB->intersectAABB(firstAABB))
            {
                firstList.push_back(object);
            }
            if(currentAABB->intersectAABB(secondAABB))
            {
                secondList.push_back(object);
            }
            //std::cout << "list 1 length" << firstList.size() << '\n';

        }
        count = count +1;
        left = new KDTreeNode(firstList, firstAABB, count);
        right = new KDTreeNode(secondList, secondAABB, count);

     }
     else
     {
         left = NULL;
         right = NULL;
         // we gucci fam
     }

 }
