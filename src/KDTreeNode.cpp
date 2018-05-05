/**
 *
 */

#include "KDTreeNode.h"

#include <glm/glm.hpp>
#include <iostream>

using namespace std;

KDTreeNode::KDTreeNode( std::vector<Object*> _objects, AABB* _aabb, int depth )
    : aabb( _aabb ) {
    objects = std::vector<Object*>( _objects );

    if ( objects.size() >= 2 && depth < MAX_DEPTH ) {
        std::vector<Object*> firstList;
        std::vector<Object*> secondList;

        AABB* firstAABB  = NULL;
        AABB* secondAABB = NULL;

        if ( depth % 3 == 0 ) {
            float xLength = ( aabb->get_max().x - aabb->get_min().x ) / 2;

            firstAABB =
                new AABB( aabb->get_max().x - xLength, aabb->get_max().y,
                          aabb->get_max().z, aabb->get_min().x,
                          aabb->get_min().y, aabb->get_min().z );
            secondAABB =
                new AABB( aabb->get_max().x, aabb->get_max().y,
                          aabb->get_max().z, aabb->get_min().x + xLength,
                          aabb->get_min().y, aabb->get_min().z );
        } else if ( depth % 3 == 1 ) {
            float yLength = ( aabb->get_max().y - aabb->get_min().y ) / 2;

            firstAABB =
                new AABB( aabb->get_max().x, aabb->get_max().y - yLength,
                          aabb->get_max().z, aabb->get_min().x,
                          aabb->get_min().y, aabb->get_min().z );
            secondAABB =
                new AABB( aabb->get_max().x, aabb->get_max().y,
                          aabb->get_max().z, aabb->get_min().x,
                          aabb->get_min().y + yLength, aabb->get_min().z );
        } else {
            float zLength = ( aabb->get_max().z - aabb->get_min().z ) / 2;

            firstAABB =
                new AABB( aabb->get_max().x, aabb->get_max().y,
                          aabb->get_max().z - zLength, aabb->get_min().x,
                          aabb->get_min().y, aabb->get_min().z );
            secondAABB =
                new AABB( aabb->get_max().x, aabb->get_max().y,
                          aabb->get_max().z, aabb->get_min().x,
                          aabb->get_min().y, aabb->get_min().z + zLength );
        }

        // std::cout << "firstAABB" << '\n';
        // firstAABB->print();
        // std::cout << "secondAABB" << '\n';
        // secondAABB->print();
        for ( Object* object : objects ) {
            AABB* currentAABB = object->getAABB();
            // std::cout << "currentAABB" << '\n';
            // currentAABB->print();
            if ( currentAABB->intersect_aabb( firstAABB ) ) {
                firstList.push_back( object );
            }
            if ( currentAABB->intersect_aabb( secondAABB ) ) {
                secondList.push_back( object );
            }
            // std::cout << "list 1 length" << firstList.size() << '\n';
        }
        depth++;
        left  = new KDTreeNode( firstList, firstAABB, depth );
        right = new KDTreeNode( secondList, secondAABB, depth );

    } else {
        // we gucci fam
        left  = NULL;
        right = NULL;
    }
}
