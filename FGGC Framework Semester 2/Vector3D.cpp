#include "Vector3D.h"


Vector3D Vector3D::operator+(const Vector3D& v) const
{
	return Vector3D(x + v.x, y + v.y, z + v.z);
}

void Vector3D::operator+=(const Vector3D& v)
{
	x += v.x;
	y += v.y;
	z += v.z; 
}

Vector3D Vector3D::operator-(const Vector3D& v) const
{
	return Vector3D(x - v.x, y - v.y, z - v.z);
}

void Vector3D::operator-=(const Vector3D& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

Vector3D Vector3D::operator*(const float value) const
{
	return Vector3D(x*value, y*value, z*value);
}


Vector3D Vector3D::operator/(const float value) const
{
	return Vector3D(x/value, y/value, z/value);
}

void Vector3D::operator*=(const float value)
{
	x *= value;
	y *= value;
	z *= value;
}

void Vector3D::operator/=(const float value)
{
	x /= value;
	y /= value;
	z /= value;
}

float Vector3D::operator*(const Vector3D& vector) const
{
	return x*vector.x + y*vector.y + z*vector.z;
}

float Vector3D::operator/(const Vector3D & vector) const
{
	return x/vector.x + y/vector.y + z/vector.z;
}

void Vector3D::operator%=(const Vector3D& vector)
{
	*this = CrossProduct(vector);
}

Vector3D Vector3D::operator%(const Vector3D& vector) const
{
	return CrossProduct(vector);
	/*return Vector3D(y*vector.z - z*vector.y,
					z*vector.x - x*vector.z,
					x*vector.y - y*vector.x);*/
}

float Vector3D::DotProduct(Vector3D & v1, Vector3D & v2)
{
	return (float) (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

Vector3D Vector3D::ComponentProduct(const Vector3D& vector) const
{
	return Vector3D(x * vector.x, y * vector.y, z * vector.z);
}

void Vector3D::ComponentProductUpdate(const Vector3D& vector)
{
	x *= vector.x;
	y *= vector.y;
	z *= vector.z;
}

Vector3D Vector3D::CrossProduct(const Vector3D& vector) const
{
	return Vector3D(y*vector.z - z*vector.y,
					z*vector.x - x*vector.z,
					x*vector.y - y*vector.x);
}

float Vector3D::ScalarProduct(const Vector3D& vector) const
{
	return x*vector.x + y*vector.y + z*vector.z;
}

void Vector3D::AddScaledVector(const Vector3D& vector, float scale)
{
	x += vector.x * scale;
	y += vector.y * scale;
	z += vector.z * scale;
}

float Vector3D::Magnitude() const
{
	return sqrt(x*x + y*y + z*z);;
}

float Vector3D::SquareMagnitude() const
{
	return x*x + y*y + z*z;
}

void Vector3D::Trim(float size)
{
	if (SquareMagnitude() > size*size)
	{
		Normalise();
		x *= size;
		y *= size;
		z *= size;
	}
}

void Vector3D::Normalise()
{
	float l = Magnitude();
	if (l > 0)
	{
		(*this) *= ((float)1) / l;
	}
}

Vector3D Vector3D::Unit() const
{
	Vector3D result = *this;
	result.Normalise();
	return result;
}

bool Vector3D::operator==(const Vector3D& other) const
{
	return  x == other.x &&
			y == other.y &&
			z == other.z;
}

bool Vector3D::operator!=(const Vector3D& other) const
{
	return !(*this == other);
}

bool Vector3D::operator<(const Vector3D& other) const
{
	return x < other.x && y < other.y && z < other.z;
}

bool Vector3D::operator>(const Vector3D& other) const
{
	return x > other.x && y > other.y && z > other.z;
}

bool Vector3D::operator<=(const Vector3D& other) const
{
	return x <= other.x && y <= other.y && z <= other.z;
}

bool Vector3D::operator>=(const Vector3D& other) const
{
	return x >= other.x && y >= other.y && z >= other.z;
}

void Vector3D::Zero()
{
	x = y = z = 0;
}

void Vector3D::Invert()
{
	x = -x;
	y = -y;
	z = -z;
}
