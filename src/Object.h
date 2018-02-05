
#include "Ray.h";
#include "Material.h";

class Object
{
	private:
		Material material;

	public :
		Object(Material mat);
		virtual float intersection(Ray ray);
		Material getMaterial();

};