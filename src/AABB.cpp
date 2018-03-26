#include "AABB.h"
#include <glm/glm.hpp>
using namespace glm;


AABB::AABB(float _x_max, float _y_max, float _z_max, float _x_min, float _y_min, float _z_min):
x_max(_x_max), y_max(_y_max), z_max(_z_max), x_min(_x_min), y_min(_y_min), z_min(_z_min) {}


bool AABB::intersectAABB(AABB* aabb)
{
    if( x_min < aabb->x_max &&
        aabb->x_min < x_max &&
        y_min < aabb->y_max &&
        aabb->y_min < y_max &&
        z_min < aabb->z_max &&
        aabb->z_min < z_max)
        return true;
    else
        return false;
}

float AABB::intersectRay(Ray* ray)
{
    vec3 dirfrac;
    // r.dir is unit direction vector of ray
    dirfrac.x = 1.0f / ray->direction->x;
    dirfrac.y = 1.0f /  ray->direction->y;
    dirfrac.z = 1.0f /  ray->direction->z;
    // lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
    // r.org is origin of ray
    float t1 = (x_min - ray->origin->x)*dirfrac.x;
    float t2 = (x_max - ray->origin->x)*dirfrac.x;
    float t3 = (y_min - ray->origin->y)*dirfrac.y;
    float t4 = (y_max - ray->origin->y)*dirfrac.y;
    float t5 = (x_min - ray->origin->z)*dirfrac.z;
    float t6 = (x_max - ray->origin->z)*dirfrac.z;

    float tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
    float tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));

    float returnValue;
    // if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
    if (tmax < 0)
    {
        return tmax;
    }

    // if tmin > tmax, ray doesn't intersect AABB
    if (tmin > tmax)
    {
        return INT_MAX;
    }
    return tmin;
}
