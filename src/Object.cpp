#include <glm/matrix.hpp>

#include "Object.h"
#include "IntersectData.h"

using namespace glm;

Object::Object( IlluminationModel* _imodel ) : imodel(_imodel) {}

void Object::transform( mat4 matrix ) {}


vec4 Object::get_color(Ray ray, float distance, std::vector<Light*> lights, vec3 ambient)
{
    IntersectData data;
    * data.position = *ray.origin + *ray.direction * distance;
    * data.normal = this->get_normal(&ray);
    * data.incoming = *ray.direction;
    data.lights = lights;
    * data.ambient = ambient;
    return vec4(0,0,0,0);

}

vec3 Object::convert( vec4 vec ) {
    return vec3( vec.x / vec.w, vec.y / vec.w, vec.z / vec.w );
}
