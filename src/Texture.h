#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <glm/vec3.hpp>
using glm::vec3;
#include "Texture.h"

class Texture {

    public:

        Texture();

        virtual vec3 get_obj_color(float u, float v) = 0;
        
};

#endif
