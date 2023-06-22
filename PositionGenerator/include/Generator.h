#pragma once
#include <random>
#include <vector>

#include "Position.h"

namespace PositionGenerator
{
	class GenerationParameter
	{
	public:
		GenerationParameter() = default;
		GenerationParameter(const GenerationParameter& Other) = default;
		GenerationParameter& operator=(const GenerationParameter& Src) = default;

		GenerationParameter& setNumOfSensors(int numOfSensors) { m_NumOfSensors = numOfSensors; return *this; }
		GenerationParameter& setBoundingCuboid(Vector3 minValues, Vector3 maxValues) { m_minValues = minValues; m_maxValues = maxValues; return *this; }
		GenerationParameter& setMaximalVelocity(float maxVelocityinMeterPerSecond) { m_maxVelocity = maxVelocityinMeterPerSecond; return *this; }
		GenerationParameter& setInitialTimestamp(timestamp_t timestamp) { m_initialTimestamp = timestamp; return *this; }
		GenerationParameter& setTimestampUnitPerSecond(uint64_t timestampUnitsPerSecond) { m_timeStampPerSecond = timestampUnitsPerSecond; return *this; }

		// read access to values
		int numOfSensors() const { return m_NumOfSensors; }
		const Vector3& minValues() const { return m_minValues; }
		const Vector3& maxValues() const { return m_maxValues; }
		float maxVelocity() const { return m_maxVelocity; }
		timestamp_t initialTimestamp() const { return m_initialTimestamp; }
		uint64_t timeStampPerSecond() const { return m_timeStampPerSecond; }

	private:
		int			m_NumOfSensors = 10; 
		Vector3 m_minValues = Vector3(0.f, 0.f, 0.5f);
		Vector3 m_maxValues = Vector3(100.f, 100.f, 1.5f);
		float		m_maxVelocity = 12.f; // 12m/s = 43,2 km/h, should be a good estimate of maximum human running speed
		timestamp_t m_initialTimestamp = 0;
		uint64_t m_timeStampPerSecond = 1000*1000; // mikroseconds to seconds
	};

	class Generator
	{
	public:
		using SensorList_t = std::vector<SensorPosition>;
		Generator(const GenerationParameter& Param);

		// allow iterating on the sensors as primary interface to them
		SensorList_t::const_iterator begin() { return m_Sensors.cbegin(); }
		SensorList_t::const_iterator end() { return m_Sensors.cend(); }

		void generateData(timestamp_t newTimestamp);

	private:
		std::random_device m_Rnd;
		std::mt19937 m_Gen;
		std::uniform_real_distribution<float> m_DistanceDist; // 0 <= v < 1
		GenerationParameter m_Param;
		std::vector<SensorPosition> m_Sensors;

		void generateNewSensorData(SensorPosition& Sensor, timestamp_t newTimestamp);
		void clamp(Vector3& Pos);
		void seedSensors();
	};

}