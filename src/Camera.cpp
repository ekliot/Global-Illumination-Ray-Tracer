/**
 * Camera.cpp
 *
 * Camera object to render a world using ray tracing
 *
 * Author: ekliot
 */

#include "Camera.h"
#include "Ray.h"

using namespace png;
using glm::vec3;

Camera::Camera( World *w, vec3 p,    vec3 l, vec3 u, PPlane pp ) : \
                 world(w), pos(p), lookat(l),  up(u), plane(pp) {
}

void Camera::render( image<rgb_pixel> *negative ) {
    vec3 ray_ori;
    vec3 ray_dir;
    Ray *ray;
    vec4 colour;
	
	vec3 n = this->pos - this->lookat;
	vec3 u = cross(this->up, n);
	u = normalize(u);
	vec3 v = cross(n, u);
	mat4 tmat = mat4(
		u.x, v.x, n.x, 0,
		u.y, v.y, n.y, 0,
		u.z, v.y, n.z, 0,
		- dot(pos, u), -dot(pos, v), -dot(pos, n),1);

	world->transform_all_to_ccoord(tmat);

    // trace and put stuff into the pixel buffer
    for ( size_t y = 0; y < negative->get_height(); ++y ) {
        for ( size_t x = 0; x < negative->get_width(); ++x ) {
            ray_ori = pos;
            ray_dir = vec3( x*plane.ratio, y*plane.ratio, plane.foc_l );
            ray = new Ray( &ray_ori, &ray_dir );
            std::cout << "spawning ray with dir: {" << ray_dir.x << "," << ray_dir.y << "," << ray_dir.z << "}" << '\n';
            colour = world->get_intersect( ray );
            std::cout << "putting colour: " << colour.x*255 << "/" << colour.y*255 << "/" << colour.z*255 << '\n';
            negative->get_row(y)[x] = rgb_pixel( int( colour.x*255 ), int( colour.y*255 ), int( colour.z*255 ) );
            // std::cout << "put colour: " << negative[y][x].red <<  '\n';
        }
    }
}
