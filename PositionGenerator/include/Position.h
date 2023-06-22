#pragma once
#include <compare>

namespace PositionGenerator
{
	using timestamp_t = uint64_t;
	using sensorId_t = uint64_t;

	// small class that represents a 3d vector
	class Vector3
	{
	public:
		explicit Vector3(float x, float y, float z) { m_x = x; m_y = y; m_z = z; }
		Vector3() = default;
		Vector3(const Vector3& Other) = default;
		Vector3& operator=(const Vector3& Other) = default;
		float x() const { return m_x; }
		float y() const { return m_y; }
		float z() const { return m_z; }

		float& x() { return m_x; }
		float& y() { return m_y; }
		float& z() { return m_z; }

		auto operator<=>(const Vector3&) const = default; 

		void normalize(); 
	private:
		float m_x = 0.0f;
		float m_y = 0.0f;
		float m_z = 0.0f;
	};

	// helper functions for 3D vector
	float scalarProduct(const Vector3& First, const Vector3& Second);
	Vector3 operator+(const Vector3& First, const Vector3& Second);
	Vector3 operator-(const Vector3& First, const Vector3& Second); 
	Vector3 operator*(const float scale, const Vector3& vector); 
	Vector3 operator*(const Vector3& vector, const float scale); 

	// wrapper around the sensor data that needs to be send out by the generator
	class SensorPosition
	{
	public:
		SensorPosition(sensorId_t SensorId, timestamp_t initialTimestamp, const Vector3& initialPosition)
			: m_sensorId(SensorId), m_timestamp(initialTimestamp), m_Position(initialPosition)
		{}

		SensorPosition() = default;
		SensorPosition(const SensorPosition& Other) = default;
		SensorPosition& operator=(const SensorPosition& Other) = default;

		sensorId_t sensorId() const { return m_sensorId; }
		void setSensorId(sensorId_t sensorId) { m_sensorId = sensorId; }
		
		timestamp_t timestamp() const { return m_timestamp; }
		void setTimestamp(timestamp_t timestamp) { m_timestamp = timestamp; }

		Vector3 position() const { return m_Position; }
		void setPosition(const Vector3& position) { m_Position = position; }

		auto operator<=>(const SensorPosition&) const = default;

	private:
		sensorId_t	m_sensorId = 0;
		timestamp_t	m_timestamp = 0;
		Vector3		m_Position;
	};

}