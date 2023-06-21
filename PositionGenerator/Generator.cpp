#include "Generator.h"

namespace PositionGenerator
{
	Generator::Generator(const GenerationParameter& Param)
		: m_Param(Param)
	{}

	void Generator::generateData(timestamp_t newTimestamp)
	{
		for (auto& Sensor : m_Sensors)
		{
			generateNewSensorData(Sensor, newTimestamp);
		}
	}

	void Generator::generateNewSensorData(SensorPosition& Sensor, timestamp_t newTimestamp)
	{
	}

	void Generator::checkBoundaries(SensorPosition& Sensor)
	{
	}
}