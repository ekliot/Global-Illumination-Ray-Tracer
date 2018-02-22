#include <glm/matrix.hpp>

#include "Object.h"
#include "IntersectData.h"

using namespace glm;

Object::Object( Material* m ) : material(m) {}

void Object::transform( mat4 matrix ) {}

vec4 Object::get_color(Ray ray, float distance, std::vector<Light> lights)
{
    IntersectData data;
    data.position = *ray.origin + *ray.direction * distance;
    data.normal = this->get_normal(&ray);
    data.incoming = *ray.direction;
    data.lights = lights;
    // return lightingModel.get_color(data);
    return this->material->color;
}

vec3 Object::convert( vec4 vec ) {
    return vec3( vec.x / vec.w, vec.y / vec.w, vec.z / vec.w );
}
