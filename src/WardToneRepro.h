#ifndef _WARD_TONE_REPRO_
#define _WARD_TONE_REPRO_

#include <glm/vec3.hpp>
#include <glm/matrix.hpp>

#include "ToneReproModel.h"

using namespace png;

class WardToneRepro : public ToneReproModel  {

public:
    WardToneRepro();
    image<rgb_pixel> Reproduce(vec3** imageBuf, float lMax);

};

#endif
