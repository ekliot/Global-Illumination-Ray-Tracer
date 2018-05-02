#include "WardToneRepro.h"
#include <iostream>

#include <math.h>
using namespace png;


WardToneRepro::WardToneRepro(): ToneReproModel(){}

image<rgb_pixel> WardToneRepro::Reproduce( vec3** imageBuf, float lMax)
{
    int ldMax = 255;
    size_t height = input->get_height();
    size_t width = input->get_width();

    float logAvgLum = getLogAverageLuminance(imageBuf);

    float temp1 = 1.219 + pow((ldMax/2.0f), 0.4f);
    float temp2 = 1.219 + pow(logAvgLum , 0.4f);
    float sf = pow((temp1/temp2),2.5f);

    image<rgb_pixel> returnImage(width, height);


    for ( size_t y = 0; y < input->get_height(); ++y ) {
        for ( size_t x = 0; x < input->get_width(); ++x ) {
            vec3 real_pixel = imageBuf[y][x];


            int red = real_pixel.x  * sf; /* pixel.red * sf /1000.0f * ldMax;*/
            int green =  real_pixel.y *sf;/*pixel.blue * sf /1000.0f* ldMax; */
            int blue =  real_pixel.z   * sf;/*pixel.green * sf /1000.0f* ldMax; */

            if(red > ldMax)
            {
                red = ldMax;
            }
            if(blue > ldMax)
            {
                blue = ldMax;
            }
            if(green > ldMax)
            {
                green = ldMax;
            }

            rgb_pixel newPixel = rgb_pixel(
                red, green, blue);

            returnImage.get_row(y)[x] = newPixel;
        }
    }

    return returnImage;
}
