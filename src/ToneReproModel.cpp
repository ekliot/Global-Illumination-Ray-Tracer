#include "ToneReproModel.h"
using namespace png;

ToneReproModel::ToneReproModel(float* _lMax):
    lMax(_lMax){}

ToneReproModel::~ToneReproModel() {
    delete lMax;
    delete input;
}

void ToneReproModel::setImage(image<rgb_pixel>* _input){
    input = _input;
}

float ToneReproModel::getLuminance(rgb_pixel pixel){

    float lum = .27f * (static_cast< float >(pixel.red)) + .67f *
    (static_cast< float >(pixel.green))  + .06f * (static_cast< float >(pixel.blue));
    return lum;
}

float ToneReproModel::getLogAverageLuminance()
{
    size_t n = input->get_height() * input->get_width();
    float total = 0;
    for ( size_t y = 0; y < input->get_height(); ++y ) {
        for ( size_t x = 0; x < input->get_width(); ++x ) {
            float newTotal = log(.000000001 + getLuminance(input->get_row(y)[x]));
            total += newTotal;
        }
    }
    total = exp(total/ n);
    return total;

}
