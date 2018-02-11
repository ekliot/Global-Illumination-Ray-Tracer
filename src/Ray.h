#ifndef _RAY_H_
#define _RAY_H_



#include <glm/vec3.hpp>

class Ray
{

	public:
		glm::vec3 point;
		glm::vec3 direction;

		Ray(glm::vec3 point, glm::vec3 direction);
		void normalize();

};
#endif // _RAY_H_