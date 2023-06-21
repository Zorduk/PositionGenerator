#pragma once
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
		Vector3 minValues() const { return m_minValues; }
		Vector3 maxValues() const { return m_maxValues; }
		float maxVelocity() const { return m_maxVelocity; }
		timestamp_t initialTimestamp() const { return m_initialTimestamp; }
		uint64_t timeStampPerSecond() const { return m_timeStampPerSecond; }

	private:
		int			m_NumOfSensors = 10; 
		Vector3 m_minValues, m_maxValues;
		float		m_maxVelocity = 12.f; // 12m/s = 43,2 km/h, should be a good estimate of human running speed
		timestamp_t m_initialTimestamp = 0;
		uint64_t m_timeStampPerSecond = 1000000; 
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
		GenerationParameter m_Param;
		std::vector<SensorPosition> m_Sensors;

		void generateNewSensorData(SensorPosition& Sensor, timestamp_t newTimestamp);
		void checkBoundaries(SensorPosition& Sensor);
	};

}