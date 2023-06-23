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
			generateWithImpulse(Sensor, newTimestamp);
		}
	}

	Vector3 Generator::addNoise(const Vector3& origPosition)
	{
		auto noiseIntensity = m_DistanceDist(m_Gen) * m_Param.noiseDimension();
		auto Noise = Vector3(m_DistanceDist(m_Gen) - 0.5f, m_DistanceDist(m_Gen) - 0.5f, 0);
		Noise.normalize();
		Noise = Noise * noiseIntensity;
		return Vector3(origPosition.x() + Noise.x(), origPosition.y() + Noise.y(), origPosition.z());
	}

	void Generator::generateWithImpulse(SensorPosition& Sensor, timestamp_t newTimestamp)
	{
		auto elapsed = newTimestamp - Sensor.timestamp();
		float timeInSec = static_cast<float>(elapsed) / static_cast<float>(m_Param.timeStampPerSecond());
		float maxDistance = m_Param.maxVelocity() * timeInSec;
		
		// some experimentation shows best results with 2.5f
		// too big and there seems to be now real movement
		// too small and all sensor end up at the border
		float maxAccelaration = maxDistance * 1.2f; 
		auto impulseVector = Sensor.velocity();

		// random acceleration
		float accFactor = m_DistanceDist(m_Gen) * maxAccelaration;
		auto accDirection = Vector3(m_DistanceDist(m_Gen) - 0.5f, m_DistanceDist(m_Gen) - 0.5f, 0);
		accDirection.normalize();
		auto acceleration = accFactor * accDirection;
		
		auto move = Sensor.velocity() + acceleration;
		// now make sure, that move is less or equal to maxVelocity
		float resultingVelo = sqrtf(scalarProduct(move, move));
		if (resultingVelo > maxDistance)
		{ 
			// if we just use maxDistance/resultingVelo as factor, we end up with rounding errors above maxDistance
			// which will fail our tests, thats why we introduce a safety factor
			constexpr float safety = 0.001f;
			move = move * ((maxDistance-safety) / resultingVelo);
		}
		auto newPos = Sensor.position() + move;
		clamp(newPos);

		// update position and timestamp for sensor
		if (timeInSec > 1.E-20f)
			Sensor.setVelocity((newPos - Sensor.position())*(1/timeInSec));
		Sensor.setPosition(newPos);
		Sensor.setTimestamp(newTimestamp);
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

		Vector3 size = m_Param.maxValues() - m_Param.minValues();
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
	
	// ChronoBasedGenerator
	ChronoBasedGenerator::ChronoBasedGenerator(const GenerationParameter& Param)
		: Generator(
			GenerationParameter(Param)
				// overwrite timestamp parameters with correct values
				.setInitialTimestamp(0)
				.setTimestampUnitPerSecond(1000000))
		, m_Start(std::chrono::high_resolution_clock::now())
	{}
	
	void ChronoBasedGenerator::generateData()
	{
		auto now = std::chrono::high_resolution_clock::now();
		timestamp_t newTime = std::chrono::duration_cast<std::chrono::microseconds>(now - m_Start).count();
		Generator::generateData(newTime);
	}
}