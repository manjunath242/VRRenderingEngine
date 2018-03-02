#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp> 
#include <glm/gtx/quaternion.hpp>
class PlaneClass
{
public:
	PlaneClass(float p1, float p2, float p3, float p4);
	~PlaneClass();
	PlaneClass NormalizePlane();
	float DotCoord(glm::vec3 point);
	float a;
	float b;
	float c;
	float d;

	PlaneClass operator*(float scalar);
	PlaneClass operator/(float scalar);
private:
	
};

