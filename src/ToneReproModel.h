
#ifndef _TONE_REPRO_MODEL_H
#define _TONE_REPRO_MODEL_H

#include <glm/vec3.hpp>
#include "png++/png.hpp"
#include <math.h>


using namespace glm;
using namespace png;

class ToneReproModel{

protected:
    float* lMax;
    image<rgb_pixel>* input;
    float getLuminance(rgb_pixel pixel);
    float getLogAverageLuminance();

public:
    ToneReproModel(float* _lMax);
    ~ToneReproModel();

    void setImage(image<rgb_pixel>* _input);
    virtual image<rgb_pixel> Reproduce() = 0;
};



#endif
