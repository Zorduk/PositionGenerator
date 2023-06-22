#include "Position.h"
#include <math.h>

namespace PositionGenerator
{
	float scalarProduct(const Vector3& First, const Vector3& Second)
	{
		return First.x() * Second.x() + First.y() * Second.y() + First.z() * Second.z();
	}
	Vector3 operator+(const Vector3& First, const Vector3& Second) 
	{ 
		return Vector3(First.x() + Second.x(), First.y() + Second.y(), First.z() + Second.z()); 
	}
	Vector3 operator-(const Vector3& First, const Vector3& Second) 
	{ 
		return Vector3(First.x() - Second.x(), First.y() - Second.y(), First.z() - Second.z()); 
	}
	Vector3 operator*(const float scale, const Vector3& vector) 
	{ 
		return Vector3(vector.x() * scale, vector.y() * scale, vector.z() * scale); 
	}
	Vector3 operator*(const Vector3& vector, const float scale) 
	{ 
		return scale * vector; 
	}
	void Vector3::normalize()
	{
		auto scProd = scalarProduct(*this, *this);
		constexpr float minNorm = 1E-20f;
		if (scProd > minNorm)
		{
			auto dist = sqrtf(scProd);
			m_x /= dist;
			m_y /= dist;
			m_z /= dist;
		}
		else
		{
			// vector too small, will not produce meaningful result
			m_x = m_y = m_z = 0;
		}
	}
}