/**
 *
 */

#include <math.h>

#include <iostream>

#include "glm/gtx/string_cast.hpp"

using namespace std;
using glm::vec3;

#include "CheckerBoard.h"

CheckerBoard::CheckerBoard( vec3 _color_odd, vec3 _color_even, int _rows,
                            int _cols )
    : Material( 0, 0 ),
      color_odd( _color_odd ),
      color_even( _color_even ),
      rows( _rows ),
      cols( _cols ) {}

vec3 CheckerBoard::get_color( float u, float v ) {
    float col_size = 1.0f / cols;
    float row_size = 1.0f / rows;

    int col = u / col_size;
    int row = v / row_size;

    int even_odd_col = col % 2;
    int even_odd_row = row % 2;

    if ( even_odd_col == even_odd_row ) {
        return color_even;
    } else {
        return color_odd;
    }
}
