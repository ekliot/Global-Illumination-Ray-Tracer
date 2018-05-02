#ifndef _REINHARD_TONE_REPRO_
#define _REINHARD_TONE_REPRO_

#include <glm/vec3.hpp>
#include <glm/matrix.hpp>

#include "ToneReproModel.h"

using namespace png;

class ReinhardToneRepro : public ToneReproModel  {

public:
    ReinhardToneRepro();
    image<rgb_pixel> Reproduce(vec3** imageBuf, float lMax);

};

#endif
