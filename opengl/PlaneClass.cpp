#include "PlaneClass.h"



PlaneClass::PlaneClass(float p1, float p2, float p3, float p4)
{
	a = p1;
	b = p2;
	c = p3;
	d = p4;
}


PlaneClass::~PlaneClass()
{
}


PlaneClass PlaneClass::NormalizePlane()
{
	PlaneClass Result(0,0,0,0);
	float Distance = sqrtf(a * a + b * b + c * c);
	Result.a = a / Distance;
	Result.b = b / Distance;
	Result.c = c / Distance;
	Result.d = d / Distance;
	return Result;
}

float PlaneClass::DotCoord(glm::vec3 point)
{
	return (a * point.x) + (b * point.y) + (c * point.z) + d;
}

PlaneClass PlaneClass::operator*(float scalar)
{
	return PlaneClass(a * scalar,
		b * scalar,
		c * scalar,
		d * scalar);
}

PlaneClass PlaneClass::operator/(float scalar)
{
	return PlaneClass(a / scalar,
		b / scalar,
		c / scalar,
		d / scalar);
}
