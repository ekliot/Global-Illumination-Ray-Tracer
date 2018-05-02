
#ifndef _TONE_REPRO_MODEL_H
#define _TONE_REPRO_MODEL_H

#include <glm/vec3.hpp>
#include "png++/png.hpp"
#include <math.h>


using namespace glm;
using namespace png;

class ToneReproModel{

protected:
    image<rgb_pixel>* input;
    float getLuminance(vec3 pixel);
    float getLogAverageLuminance(vec3 ** imageBuf);

public:
    ToneReproModel();
    ~ToneReproModel();

    void setImage(image<rgb_pixel>* _input);
    virtual image<rgb_pixel> Reproduce(vec3** imageBuf, float lMax) = 0;
};



#endif
