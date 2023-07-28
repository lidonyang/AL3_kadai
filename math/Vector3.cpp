#include "Vector3.h"

Vector3& operator*=(Vector3& v, float s) 
{ 
	v.x *= s;
	v.y *= s;
	v.z *= s;
	return v;
}

const Vector3 operator*(const Vector3& v, float s) 
{ 
	Vector3 temp(v);
	return temp *= s;
}
