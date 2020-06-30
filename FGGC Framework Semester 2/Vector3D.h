#pragma once
#include <cmath>
#include <iostream>
#include <string>
#include <DirectXMath.h>


using namespace DirectX;

class Vector3D
{

public:
	float x;
	float y;
	float z;

	Vector3D():x(0.0f), y(0.0f), z(0.0f){}
	Vector3D(const float x, const float y, const float z) : x(x), y(y), z(z) {}

	Vector3D operator+(const Vector3D& v) const;
	void operator+=(const Vector3D& v);
	Vector3D operator-(const Vector3D& v) const;
	void operator-=(const Vector3D& v);
	void operator*=(const float value);
	void operator/=(const float value);
	float operator *(const Vector3D &vector) const;
	float operator /(const Vector3D &vector) const;
	Vector3D operator*(const float value) const;
	Vector3D operator/(const float value) const;
	void operator%=(const Vector3D &vector);
	Vector3D operator%(const Vector3D &vector) const;

	float Distance(const Vector3D& v)
	{
		Vector3D vecSeperation = v - *this;
		
		return vecSeperation.Magnitude();
	}

	static float DotProduct(Vector3D& v1, Vector3D& v2);

	Vector3D ComponentProduct(const Vector3D &vector) const;
	void ComponentProductUpdate(const Vector3D &vector);
	Vector3D CrossProduct(const Vector3D &vector) const;
	float ScalarProduct(const Vector3D &vector) const;
	void AddScaledVector(const Vector3D &vector, float scale);
	float Magnitude() const;
	float SquareMagnitude() const;
	//Limits the size of the vector to the given maxium
	void Trim(float size);
	void Normalise();
	Vector3D Unit() const;

	bool operator==(const Vector3D& other) const;
	bool operator!=(const Vector3D& other) const;
	bool operator<(const Vector3D& other) const;
	bool operator>(const Vector3D& other) const;
	bool operator<=(const Vector3D& other) const;
	bool operator>=(const Vector3D& other) const;
	void Zero();
	void Invert();
};

class Quaternion
{
public:
	float r;
	
	float i;

	float j;

	float k;

	Quaternion() : r(1), i(0), j(0), k(0) {}

	Quaternion(const float r, const float i, const float j, const float k)
		: r(r), i(i), j(j), k(k)
	{
	}

	void Normalise()
	{
		float d = r * r + i * i + j * j + k * k;

		// quaternion in that case.
		if (d < FLT_EPSILON)
		{
			r = 1;
			return;
		}

		d = static_cast<float>(1.0) / sqrtf(d);
		r *= d;
		i *= d;
		j *= d;
		k *= d;
	}

	void operator *=(const Quaternion &multiplier)
	{
		Quaternion q = *this;
		r = q.r*multiplier.r - q.i*multiplier.i -
			q.j*multiplier.j - q.k*multiplier.k;
		i = q.r*multiplier.i + q.i*multiplier.r +
			q.j*multiplier.k - q.k*multiplier.j;
		j = q.r*multiplier.j + q.j*multiplier.r +
			q.k*multiplier.i - q.i*multiplier.k;
		k = q.r*multiplier.k + q.k*multiplier.r +
			q.i*multiplier.j - q.j*multiplier.i;
	}

	void addScaledVector(const Vector3D& vector, float scale)
	{
		Quaternion q(0,
			vector.x * scale,
			vector.y * scale,
			vector.z * scale);
		q *= *this;
		r += q.r * 0.5f;
		i += q.i * 0.5f;
		j += q.j * 0.5f;
		k += q.k * 0.5f;
	}

	void rotateByVector(const Vector3D& vector)
	{
		Quaternion q(0, vector.x, vector.y, vector.z);
		(*this) *= q;
	}

	Quaternion EulerToQuaternion(float pitch, float roll, float yaw)
	{
		Quaternion q;
		double cy = cos(yaw * 0.5);
		double sy = sin(yaw * 0.5);
		double cr = cos(roll * 0.5);
		double sr = sin(roll * 0.5);
		double cp = cos(pitch * 0.5);
		double sp = sin(pitch * 0.5);

		q.r = cy * cr * cp + sy * sr * sp;
		q.i = cy * sr * cp - sy * cr * sp;
		q.j = cy * cr * sp + sy * sr * cp;
		q.k = sy * cr * cp - cy * sr * sp;

		return q;
	}
};

static inline XMMATRIX CalculateTransformMatrix(XMMATRIX &transformMatrix,
	const Vector3D &position,
	const Quaternion &orientation)
{
	transformMatrix.r[0] = XMVectorSetX(transformMatrix.r[0], 1 - 2 * orientation.j*orientation.j -
		2 * orientation.k*orientation.k);
	transformMatrix.r[0] = XMVectorSetY(transformMatrix.r[0], 2 * orientation.i*orientation.j -
		2 * orientation.r*orientation.k);
	transformMatrix.r[0] = XMVectorSetZ(transformMatrix.r[0], 2 * orientation.i*orientation.k +
		2 * orientation.r*orientation.j);
	transformMatrix.r[0] = XMVectorSetW(transformMatrix.r[0], 0.0f);

	transformMatrix.r[1] = XMVectorSetX(transformMatrix.r[1], 2 * orientation.i*orientation.j +
		2 * orientation.r*orientation.k);
	transformMatrix.r[1] = XMVectorSetY(transformMatrix.r[1], 1 - 2 * orientation.i*orientation.i -
		2 * orientation.k*orientation.k);
	transformMatrix.r[1] = XMVectorSetZ(transformMatrix.r[1], 2 * orientation.j*orientation.k -
		2 * orientation.r*orientation.i);
	transformMatrix.r[1] = XMVectorSetW(transformMatrix.r[1], 0.0f);

	transformMatrix.r[2] = XMVectorSetX(transformMatrix.r[2], 2 * orientation.i*orientation.k -
		2 * orientation.r*orientation.j);
	transformMatrix.r[2] = XMVectorSetY(transformMatrix.r[2], 2 * orientation.j*orientation.k +
		2 * orientation.r*orientation.i);
	transformMatrix.r[2] = XMVectorSetZ(transformMatrix.r[2], 1 - 2 * orientation.i*orientation.i -
		2 * orientation.j*orientation.j);
	transformMatrix.r[2] = XMVectorSetW(transformMatrix.r[2], 0.0f);

	transformMatrix.r[3] = XMVectorSetX(transformMatrix.r[3], position.x);
	transformMatrix.r[3] = XMVectorSetY(transformMatrix.r[3], position.y);
	transformMatrix.r[3] = XMVectorSetZ(transformMatrix.r[3], position.z);
	transformMatrix.r[3] = XMVectorSetW(transformMatrix.r[3], 1.0f);

	return transformMatrix;
}


struct CollisionPrimitive
{
public:
	std::string _type;
	void SetType(std::string type) { _type = type; }
	std::string GetType() const { return _type; }

};

struct Sphere : public CollisionPrimitive
{
	float _radius;

	void SetRadius(float radius) { _radius = radius; }
	float GetRadius()const { return _radius; }
};

struct Plane : public CollisionPrimitive
{
	Vector3D _direction;
	float _offset;
	
};