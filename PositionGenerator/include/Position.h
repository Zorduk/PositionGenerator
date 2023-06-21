#pragma once
#include <compare>

namespace PositionGenerator
{

	// small storage class that encapsulates a 3d value
	class Vector3
	{
	public:
		explicit Vector3(float x, float y, float z) { m_x = x; m_y = y; m_z = z; }
		Vector3() = default;
		Vector3(const Vector3& Other) = default;
		Vector3& operator=(Vector3& Other) = default;
	
		float distance(const Vector3& Other) const; // slow compared to distanceSquared
		float distanceSquared(const Vector3& Other) const;

		float x() const { return m_x; }
		float y() const { return m_y; }
		float z() const { return m_z; }

		float& x() { return m_x; }
		float& y() { return m_y; }
		float& z() { return m_z; }

		auto operator<=>(const Vector3&) const = default; 

	private:
		float m_x = 0.0f;
		float m_y = 0.0f;
		float m_z = 0.0f;
	};

	// helper functions
	float scalarProduct(const Vector3& First, const Vector3& Second);
	Vector3 operator+(const Vector3& First, const Vector3& Second);
	Vector3 operator-(const Vector3& First, const Vector3& Second); 
	Vector3 operator*(const float scale, const Vector3& vector); 
	Vector3 operator*(const Vector3& vector, const float scale); 
}