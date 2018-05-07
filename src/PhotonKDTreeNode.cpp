/**
 *
 */

#include "PhotonKDTreeNode.h"

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <iostream>

PhotonKDTreeNode::PhotonKDTreeNode( vector<Photon*> _photons ) {
    vec3 v_max = vec3( FLT_MIN );
    vec3 v_min = vec3( FLT_MAX );

    using std::max;
    using std::min;

    for ( Photon* p : _photons ) {
        v_max.x = max( v_max.x, p->position.x );
        v_max.y = max( v_max.y, p->position.y );
        v_max.z = max( v_max.z, p->position.z );

        v_min.x = min( v_min.x, p->position.x );
        v_min.y = min( v_min.y, p->position.y );
        v_min.z = min( v_min.z, p->position.z );
    }

    AABB* _aabb = new AABB( v_max, v_min );

    PhotonKDTreeNode( _photons, _aabb, 0 );
}

PhotonKDTreeNode::PhotonKDTreeNode( vector<Photon*> _photons, AABB* _aabb,
                                    int depth ) {
    aabb    = _aabb;
    photons = vector<Photon*>( _photons );
    std::cout << "photons // " << photons.size() << '\n';

    using std::abs;
    using std::max;
    using std::min;

    if ( photons.size() > 1 && depth < MAX_DEPTH ) {
        vector<Photon*> list_left  = vector<Photon*>();
        vector<Photon*> list_right = vector<Photon*>();

        vec3 v_max = vec3( FLT_MIN );
        vec3 v_min = vec3( FLT_MAX );

        for ( Photon* photon : _photons ) {
            v_max.x = max( v_max.x, photon->position.x );
            v_max.y = max( v_max.y, photon->position.y );
            v_max.z = max( v_max.z, photon->position.z );

            v_min.x = min( v_min.x, photon->position.x );
            v_min.y = min( v_min.y, photon->position.y );
            v_min.z = min( v_min.z, photon->position.z );
        }

        float dx = abs( v_min.x - v_max.x );
        float dy = abs( v_min.y - v_max.y );
        float dz = abs( v_min.z - v_max.z );

        float len;
        AABB* aabb_left;
        AABB* aabb_right;

        // make aabbs here
        if ( dx > dy && dx > dz ) {
            axis = 0;

            len = ( aabb->get_max().x - aabb->get_min().x ) / 2;

            aabb_left  = new AABB( aabb->get_max().x - len, aabb->get_max().y,
                                  aabb->get_max().z, aabb->get_min().x,
                                  aabb->get_min().y, aabb->get_min().z );
            aabb_right = new AABB( aabb->get_max().x, aabb->get_max().y,
                                   aabb->get_max().z, aabb->get_min().x + len,
                                   aabb->get_min().y, aabb->get_min().z );
        } else if ( dy > dx && dy > dz ) {
            axis = 1;

            len = ( aabb->get_max().y - aabb->get_min().y ) / 2;

            aabb_left  = new AABB( aabb->get_max().x, aabb->get_max().y - len,
                                  aabb->get_max().z, aabb->get_min().x,
                                  aabb->get_min().y, aabb->get_min().z );
            aabb_right = new AABB( aabb->get_max().x, aabb->get_max().y,
                                   aabb->get_max().z, aabb->get_min().x,
                                   aabb->get_min().y + len, aabb->get_min().z );
        } else {
            axis = 2;

            len = ( aabb->get_max().z - aabb->get_min().z ) / 2;

            aabb_left  = new AABB( aabb->get_max().x, aabb->get_max().y,
                                  aabb->get_max().z - len, aabb->get_min().x,
                                  aabb->get_min().y, aabb->get_min().z );
            aabb_right = new AABB( aabb->get_max().x, aabb->get_max().y,
                                   aabb->get_max().z, aabb->get_min().x,
                                   aabb->get_min().y, aabb->get_min().z + len );
        }

        for ( Photon* p : photons ) {
            if ( aabb_left->intersect_point( p->position ) ) {
                list_left.push_back( p );
            }
            if ( aabb_right->intersect_point( p->position ) ) {
                list_right.push_back( p );
            }
        }

        depth++;

        left  = new PhotonKDTreeNode( list_left, aabb_left, depth );
        right = new PhotonKDTreeNode( list_right, aabb_right, depth );

    } else {
        axis  = -1;
        left  = NULL;
        right = NULL;
    }
}

PhotonKDTreeNode::~PhotonKDTreeNode() {
    delete left;
    delete right;
    delete aabb;
}

void PhotonKDTreeNode::get_photons_near_pt( PhotonHeap* heap, vec3 position,
                                            float range ) {
    if ( left != NULL ) {
        float delta = 0;

        // calculate distance
        if ( axis == 0 ) {
            delta = left->aabb->get_max().x - position.x;
        } else if ( axis == 1 ) {
            delta = left->aabb->get_max().y - position.y;
        } else if ( axis == 2 ) {
            delta = left->aabb->get_max().z - position.z;
        }

        if ( delta < 0 ) {
            left->get_photons_near_pt( heap, position, range );
            if ( pow( delta, 2 ) > pow( range, 2 ) ) {
                right->get_photons_near_pt( heap, position, range );
            }
        } else {
            right->get_photons_near_pt( heap, position, range );
            if ( pow( delta, 2 ) > pow( range, 2 ) ) {
                left->get_photons_near_pt( heap, position, range );
            }
        }
    } else {
        for ( Photon* old_p : photons ) {
            Photon new_p = {};

            new_p.position = vec3( old_p->position );
            new_p.power    = vec3( old_p->power );
            new_p.dir      = vec3( old_p->dir );
            new_p.distance = glm::distance( new_p.position, position );

            heap->push( &new_p );
        }
    }
}

void PhotonKDTreeNode::get_n_photons_near_pt( PhotonHeap* heap, vec3 position,
                                              size_t size ) {
    if ( left != NULL && heap->size() < size ) {
        float delta = 0.0f;

        // calculate distance
        if ( axis == 0 ) {
            delta = left->aabb->get_max().x - position.x;
        } else if ( axis == 1 ) {
            delta = left->aabb->get_max().y - position.y;
        } else if ( axis == 2 ) {
            delta = left->aabb->get_max().z - position.z;
        }

        if ( delta < 0 ) {
            left->get_n_photons_near_pt( heap, position, size );
            right->get_n_photons_near_pt( heap, position, size );
        } else {
            right->get_n_photons_near_pt( heap, position, size );
            left->get_n_photons_near_pt( heap, position, size );
        }
    } else {
        std::cout << "/*" << '\n';
        for ( Photon* old_p : photons ) {
            if ( heap->size() <= size ) {
                Photon new_p = {};

                new_p.position  = vec3( old_p->position );
                new_p.power     = vec3( old_p->power );
                new_p.dir       = vec3( old_p->dir );
                new_p.distance  = glm::distance( new_p.position, position );
                new_p.is_shadow = old_p->is_shadow;

                heap->push( &new_p );
            } else {
                break;
            }
        }
    }
}
