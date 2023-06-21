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
	float Vector3::distance(const Vector3& Other) const
	{
		return sqrtf(distanceSquared(Other));
	}
	float Vector3::distanceSquared(const Vector3& Other) const
	{
		Vector3 distV3 = *this - Other;
		return scalarProduct(distV3, distV3);
	}

	void Vector3::normalize()
	{
		auto scProd = scalarProduct(*this, *this);
		if (scProd > 1.E-10)
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