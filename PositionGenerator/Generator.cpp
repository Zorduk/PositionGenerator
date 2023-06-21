#include "Generator.h"

namespace PositionGenerator
{
	Generator::Generator(const GenerationParameter& Param)
		: m_Param(Param), m_Gen(m_Rnd())
	{
		seedSensors();
	}

	void Generator::generateData(timestamp_t newTimestamp)
	{
		for (auto& Sensor : m_Sensors)
		{
			generateNewSensorData(Sensor, newTimestamp);
		}
	}

	void Generator::generateNewSensorData(SensorPosition& Sensor, timestamp_t newTimestamp)
	{
		auto elapsed = newTimestamp - Sensor.timestamp();
		float timeInSec = static_cast<float>(elapsed) / static_cast<float>(m_Param.timeStampPerSecond());
		float maxDistance = m_Param.maxVelocity() * timeInSec;

		// random distance within velocity parameter
		float moveDistance = m_DistanceDist(m_Gen) * maxDistance;

		// random x-y-direction, z is constant 
		auto moveDirection = Vector3(m_DistanceDist(m_Gen), m_DistanceDist(m_Gen), 0);
		moveDirection.normalize();
		auto move = moveDistance * moveDirection;
		auto newPos = Sensor.position() + move;
		clamp(newPos);
	}

	void Generator::clamp(Vector3& Pos)
	{
		const Vector3& minV = m_Param.minValues();
		const Vector3& maxV = m_Param.maxValues();
		Pos.x() = std::clamp(Pos.x(), minV.x(), maxV.x());
		Pos.y() = std::clamp(Pos.y(), minV.y(), maxV.y());
		Pos.z() = std::clamp(Pos.z(), minV.z(), maxV.z());
	}

	void Generator::seedSensors()
	{
		// for safety, if seedSensors get called outside ctor
		m_Sensors.clear();

		Vector3 size = m_Param.maxValues() + m_Param.minValues();
		for (int i = 0; i < m_Param.numOfSensors(); ++i)
		{
			Vector3 randomPosWithinSize(
				m_DistanceDist(m_Gen) * size.x(),
				m_DistanceDist(m_Gen) * size.y(),
				m_DistanceDist(m_Gen) * size.z());
			auto randomPosWithinBounds = m_Param.minValues() + randomPosWithinSize;
			m_Sensors.emplace_back(i, m_Param.initialTimestamp(), randomPosWithinBounds);
		}
	}
}