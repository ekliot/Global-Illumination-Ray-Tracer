#include "AABB.h"
#include <glm/glm.hpp>
using namespace glm;


AABB::AABB(float _x_max, float _y_max, float _z_max, float _x_min, float _y_min, float _z_min):
x_max(_x_max), y_max(_y_max), z_max(_z_max), x_min(_x_min), y_min(_y_min), z_min(_z_min) {}

AABB::AABB(vec3 max, vec3 min):
x_max(max.x), y_max(max.y), z_max(max.z), x_min(min.x), y_min(min.y), z_min(min.z) {}

AABB::AABB(AABB* first, AABB* second)
{
    x_max = max(first->x_max, second->x_max);
    y_max = max(first->y_max, second->y_max);
    z_max = max(first->z_max, second->z_max);

    x_min = min(first->x_min, second->x_min);
    y_min = min(first->y_min, second->y_min);
    z_min = min(first->z_min, second->z_min);
}

bool AABB::intersectAABB(AABB* aabb)
{

    // if (Aaxis.min > Baxis.max)
    // or
    //    (Baxis.min > Aaxis.max) return FALSE

    if( x_min > aabb->x_max ||
        aabb->x_min > x_max ||
        y_min > aabb->y_max ||
        aabb->y_min > y_max ||
        z_min > aabb->z_max ||
        aabb->z_min > z_max
     )
        return false;

    return true;
    // if( x_min < aabb->x_max &&
    //     aabb->x_min < x_max &&
    //     y_min < aabb->y_max &&
    //     aabb->y_min < y_max &&
    //     z_min < aabb->z_max &&
    //     aabb->z_min < z_max)
    //
    //     return true;
    // else
    //     return false;
}

float AABB::intersectRay(Ray* ray)
{
    vec3 dirfrac;
    // r.dir is unit direction vector of ray
    dirfrac.x = 1.0f / ray->direction->x;
    dirfrac.y = 1.0f / ray->direction->y;
    dirfrac.z = 1.0f /ray->direction->z;

    // lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
    // r.org is origin of ray
    float t1 = (x_min - ray->origin->x)*dirfrac.x;
    float t2 = (x_max - ray->origin->x)*dirfrac.x;
    float t3 = (y_min - ray->origin->y)*dirfrac.y;
    float t4 = (y_max - ray->origin->y)*dirfrac.y;
    float t5 = (z_min - ray->origin->z)*dirfrac.z;
    float t6 = (z_max - ray->origin->z)*dirfrac.z;

    float tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
    float tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));

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
    return tmax;
}

vec3 AABB::getMax()
{
    return vec3(x_max, y_max, z_max);
}

vec3 AABB::getMin()
{
    return vec3(x_min, y_min, z_min);
}

void AABB::print()
{
    std::cout << "xmax: " << x_max << ' ';
    std::cout << "ymax: " << y_max << ' ';
    std::cout << "zmax: " << z_max << '\n';
    std::cout << "xmin: " << x_min << ' ';
    std::cout << "ymin: " << y_min << ' ';
    std::cout << "zmin: " << z_min << '\n';
}
