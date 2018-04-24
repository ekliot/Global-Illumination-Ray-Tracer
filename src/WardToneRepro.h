#ifndef _WARD_TONE_REPRO_
#define _WARD_TONE_REPRO_

#include <glm/vec3.hpp>
#include <glm/matrix.hpp>

#include "ToneReproModel.h"

using namespace png;

class WardToneRepro : public ToneReproModel  {

public:
    WardToneRepro(float* _lMax);
    image<rgb_pixel> Reproduce();

};

#endif
