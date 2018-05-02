#include "ToneReproModel.h"
using namespace png;

ToneReproModel::ToneReproModel(){

}

ToneReproModel::~ToneReproModel() {
    delete input;
}

void ToneReproModel::setImage(image<rgb_pixel>* _input){
    input = _input;
}

float ToneReproModel::getLuminance(vec3 pixel){

    float lum = (.27f * pixel.x) + (.67f * pixel.y)  + (.06f * pixel.z);
    return lum;
}

float ToneReproModel::getLogAverageLuminance(vec3 ** imageBuf)
{
    size_t n = input->get_height() * input->get_width();
    float total = 0;
    for ( size_t y = 0; y < input->get_height(); ++y ) {
        for ( size_t x = 0; x < input->get_width(); ++x ) {
            float newTotal = log(.000000001 + getLuminance(imageBuf[y][x]) );
            total += newTotal;
        }
    }
    total = total /n;
    total = exp(total);
    return total;

}
