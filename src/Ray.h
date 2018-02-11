#include <glm/vec3.hpp>

class Ray {
public:
	glm::vec3 point;
	glm::vec3 direction;

	Ray( glm::vec3 point, glm::vec3 direction );
	void Normalize();

};
