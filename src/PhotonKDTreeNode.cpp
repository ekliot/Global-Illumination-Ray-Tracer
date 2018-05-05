
#include "PhotonKDTreeNode.h"
#include <glm/glm.hpp>
#include <iostream>
using namespace std;

PhotonKDTreeNode::PhotonKDTreeNode(std::vector<Photon> _photons, AABB* _aabb, int count)
:aabb(_aabb)
{
    photons = new vector<Photon>();
    for(Photon photon : _photons)
    {
        photons->push_back(photon);
    }

    if(photons->size() < 2 && count < 25)
    {
        vector<Photon> firstList = vector<Photon>();
        vector<Photon> secondList = vector<Photon>();

        vec3 maxVec = vec3(FLT_MIN);
        vec3 minVec = vec3(FLT_MAX);
        for(Photon photon : _photons)
        {
            maxVec.x = std::max(maxVec.x, photon.position->x);
            maxVec.y = std::max(maxVec.y, photon.position->y);
            maxVec.z = std::max(maxVec.z, photon.position->z);

            minVec.x = std::min(minVec.x, photon.position->x);
            minVec.y = std::min(minVec.y, photon.position->y);
            minVec.z = std::min(minVec.z, photon.position->z);
        }
        float deltaX = abs(minVec.x - maxVec.x);
        float deltaY = abs(minVec.y - maxVec.y);
        float deltaZ = abs(minVec.z - maxVec.z);

        AABB* firstAABB = NULL;
        AABB* secondAABB = NULL;

        // make aabbs here
        if(deltaX > deltaY && deltaX > deltaZ)
        {
            axis = 0;

            float xLength = (aabb->getMax().x - aabb->getMin().x)/2;

            firstAABB = new AABB(aabb->getMax().x - xLength, aabb->getMax().y, aabb->getMax().z,
            aabb->getMin().x, aabb->getMin().y, aabb->getMin().z);
            secondAABB = new AABB(aabb->getMax().x, aabb->getMax().y, aabb->getMax().z,
            aabb->getMin().x + xLength, aabb->getMin().y, aabb->getMin().z);
        }
        else if( deltaY > deltaX && deltaY > deltaZ)
        {
            axis = 1;

            float yLength = (aabb->getMax().y - aabb->getMin().y)/2;

            firstAABB = new AABB(aabb->getMax().x, aabb->getMax().y - yLength, aabb->getMax().z,
            aabb->getMin().x, aabb->getMin().y, aabb->getMin().z);
            secondAABB = new AABB(aabb->getMax().x, aabb->getMax().y, aabb->getMax().z,
            aabb->getMin().x, aabb->getMin().y + yLength, aabb->getMin().z);
        }
        else
        {
            axis = 2;

            float zLength = (aabb->getMax().z - aabb->getMin().z)/2;

            firstAABB = new AABB(aabb->getMax().x, aabb->getMax().y, aabb->getMax().z - zLength,
            aabb->getMin().x, aabb->getMin().y, aabb->getMin().z);
            secondAABB = new AABB(aabb->getMax().x, aabb->getMax().y, aabb->getMax().z,
            aabb->getMin().x, aabb->getMin().y, aabb->getMin().z + zLength);
        }

        for(size_t i=0; i < photons->size(); i++){
            Photon currentPhoton = photons->at(i);
            if(firstAABB->intersectPoint(* currentPhoton.position))
            {
                firstList.push_back(currentPhoton);
            }
            if(secondAABB->intersectPoint(* currentPhoton.position))
            {
                secondList.push_back(currentPhoton);
            }
        }
        count++;

        left = new PhotonKDTreeNode(firstList, firstAABB, count);
        right = new PhotonKDTreeNode(secondList, secondAABB, count);

    }
    else
    {
        left = NULL;
        right = NULL;
    }


}
