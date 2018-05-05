
#include "PhotonKDTreeNode.h"

#include <glm/glm.hpp>
#include <iostream>
using namespace std;

PhotonKDTreeNode::PhotonKDTreeNode( std::vector<Photon*> _photons, AABB* _aabb,
                                    int depth )
    : aabb( _aabb ) {
    photons = vector<Photon*>( _photons );

    if ( photons.size() < 2 && depth < MAX_DEPTH ) {
        vector<Photon*> list_left = vector<Photon*>();
        vector<Photon*> list_rite = vector<Photon*>();

        vec3 maxVec = vec3( FLT_MIN );
        vec3 minVec = vec3( FLT_MAX );

        for ( Photon* photon : _photons ) {
            maxVec.x = std::max( maxVec.x, photon->position.x );
            maxVec.y = std::max( maxVec.y, photon->position.y );
            maxVec.z = std::max( maxVec.z, photon->position.z );

            minVec.x = std::min( minVec.x, photon->position.x );
            minVec.y = std::min( minVec.y, photon->position.y );
            minVec.z = std::min( minVec.z, photon->position.z );
        }

        float dx = abs( minVec.x - maxVec.x );
        float dy = abs( minVec.y - maxVec.y );
        float dz = abs( minVec.z - maxVec.z );

        AABB* aabb_left  = NULL;
        AABB* aabb_right = NULL;

        // make aabbs here
        axis = set_aabbs( aabb, aabb_left, aabb_right, dx, dy, dz );

        for ( size_t i = 0; i < photons.size(); i++ ) {
            Photon* p = photons.at( i );
            if ( aabb_left->intersect_point( p->position ) ) {
                list_left.push_back( p );
            }
            if ( aabb_right->intersect_point( p->position ) ) {
                list_rite.push_back( p );
            }
        }
        depth++;

        left  = new PhotonKDTreeNode( list_left, aabb_left, depth );
        right = new PhotonKDTreeNode( list_rite, aabb_right, depth );

    } else {
        left  = NULL;
        right = NULL;
    }
}

// returns the splitting axis
int set_aabbs( AABB* src, AABB* left, AABB* right, float dx, float dy,
               float dz ) {
    int axis;
    float len;

    // make aabbs here
    if ( dx > dy && dx > dz ) {
        axis = 0;

        len = ( src->get_max().x - src->get_min().x ) / 2;

        left  = new AABB( src->get_max().x - len, src->get_max().y,
                         src->get_max().z, src->get_min().x, src->get_min().y,
                         src->get_min().z );
        right = new AABB( src->get_max().x, src->get_max().y, src->get_max().z,
                          src->get_min().x + len, src->get_min().y,
                          src->get_min().z );
    } else if ( dy > dx && dy > dz ) {
        axis = 1;

        len = ( src->get_max().y - src->get_min().y ) / 2;

        left  = new AABB( src->get_max().x, src->get_max().y - len,
                         src->get_max().z, src->get_min().x, src->get_min().y,
                         src->get_min().z );
        right = new AABB( src->get_max().x, src->get_max().y, src->get_max().z,
                          src->get_min().x, src->get_min().y + len,
                          src->get_min().z );
    } else {
        axis = 2;

        len = ( src->get_max().z - src->get_min().z ) / 2;

        left  = new AABB( src->get_max().x, src->get_max().y,
                         src->get_max().z - len, src->get_min().x,
                         src->get_min().y, src->get_min().z );
        right = new AABB( src->get_max().x, src->get_max().y, src->get_max().z,
                          src->get_min().x, src->get_min().y,
                          src->get_min().z + len );
    }

    return axis;
}

PhotonKDTreeNode::~PhotonKDTreeNode() {
    delete left;
    delete right;
    delete aabb;
}
