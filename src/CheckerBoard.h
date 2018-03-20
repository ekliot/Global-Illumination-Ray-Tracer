
#ifndef _CHECKERBOARD_H_
#define _CHECKERBOARD_H_

#include <glm/vec3.hpp>
using glm::vec3;

#include "Material.h"

class CheckerBoard : public Material {
    vec3 color_odd;
    vec3 color_even;

    int rows;
    int cols;

    public:
        CheckerBoard( vec3 _color_odd, vec3 _color_even, int _rows, int _cols );

        vec3 get_color( float u, float v );
};
#endif
