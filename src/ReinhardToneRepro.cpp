#include "ReinhardToneRepro.h"
#include <iostream>

#include <math.h>
using namespace png;


ReinhardToneRepro::ReinhardToneRepro(float* _lMax): ToneReproModel(_lMax){}

image<rgb_pixel> ReinhardToneRepro::Reproduce()
{
    int ldMax = 255;
    size_t height = input->get_height();
    size_t width = input->get_width();

    float logAvgLum = getLogAverageLuminance();

    // float temp1 = 1.219 + pow((ldMax/2.0f), 0.4f);
    // float temp2 = 1.219 + pow(logAvgLum , 0.4f);
    // float sf = pow((temp1/temp2),2.5f);


    image<rgb_pixel> returnImage(width, height);


    for ( size_t y = 0; y < input->get_height(); ++y ) {
        for ( size_t x = 0; x < input->get_width(); ++x ) {
            rgb_pixel pixel = input->get_row(y)[x];

            float sScaler = .18/logAvgLum;
            float rS = sScaler * pixel.red;
            float gS = sScaler * pixel.green;
            float bS = sScaler * pixel.blue;

            float rR = rS/(1+rS);
            float gR = gS/(1+gS);
            float bR = bS/(1+bS);

            float red = rR * ldMax;
            float green = gR * ldMax;
            float blue = bR * ldMax;

            if(red > ldMax)
            {
                red = ldMax;
            }
            if(green > ldMax)
            {
                green = ldMax;
            }
            if(blue > ldMax)
            {
                blue = ldMax;
            }


            rgb_pixel newPixel = rgb_pixel(
                red, green, blue);

            returnImage.get_row(y)[x] = newPixel;
        }
    }

    return returnImage;
}
