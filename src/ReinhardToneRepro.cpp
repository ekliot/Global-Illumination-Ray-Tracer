#include "ReinhardToneRepro.h"
#include <iostream>

#include <math.h>
using namespace png;


ReinhardToneRepro::ReinhardToneRepro(): ToneReproModel(){}

image<rgb_pixel> ReinhardToneRepro::Reproduce(vec3** imageBuf, float lMax)
{
    float ldMax = 255.0f;
    size_t height = input->get_height();
    size_t width = input->get_width();

    float logAvgLum = getLogAverageLuminance(imageBuf);

    image<rgb_pixel> returnImage(width, height);

    std::cout << "logAvgLum: " <<logAvgLum;


    for ( size_t y = 0; y < input->get_height(); ++y ) {
        for ( size_t x = 0; x < input->get_width(); ++x ) {
            vec3 real_pixel = imageBuf[y][x];

            float sScaler = .18/logAvgLum;
            float rS = sScaler * real_pixel.x;
            float gS = sScaler * real_pixel.y;
            float bS = sScaler * real_pixel.z;

            float rR = rS/(1+rS);
            float gR = gS/(1+gS);
            float bR = bS/(1+bS);

            float red = rR * ldMax;
            float green = gR * ldMax;
            float blue = bR * ldMax;

            // std::cout <<" red : "<<red;
            // std::cout <<" green : "<<green;
            // std::cout <<" blue : "<<blue << "\n";


            if(red > ldMax)
            {
                std::cout <<"oof 1 \n";
                red = ldMax;
            }
            if(red < 0)
            {
                std::cout <<"oof 1 \n";
                red = 0;
            }
            if(green > ldMax)
            {
                std::cout <<"oof 1 \n";
                green = ldMax;
            }
            if(green < 0)
            {
                std::cout <<"oof 1 \n";
                green = 0;
            }
            if(blue > ldMax)
            {
                std::cout <<"oof 1 \n";
                blue = ldMax;
            }
            if(blue < 0)
            {
                std::cout <<"oof 1 \n";
                blue = 0;
            }



            rgb_pixel newPixel = rgb_pixel(
                red, green, blue);

            returnImage.get_row(y)[x] = newPixel;
        }
    }

    return returnImage;
}
