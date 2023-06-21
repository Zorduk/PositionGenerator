#include "Position.h"
#include <math.h>

namespace PositionGenerator
{

	/////////////////
	// Vector3 
	////////////////
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
	/////////////////
	// Vector3 
	////////////////


}