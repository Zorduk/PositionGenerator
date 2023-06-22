#include <set>

#include "gtest/gtest.h"

#include "Position.h"
#include "Generator.h"


TEST(Generator, SensorSeed)
{
	using namespace PositionGenerator;

	float maxVelocity = 10.0; // 10m/s
	uint64_t timeStampUnitsPerSecond = 1; // we use seconds now
	timestamp_t initialTime = 10; // we start at 10sec
	Vector3 minValues(10.f, 10.f, 0.2f);
	Vector3 maxValues(110.f, 110.f, 1.5f);
	int numSensors = 120;

	Generator Gen(GenerationParameter()
		.setMaximalVelocity(maxVelocity)
		.setNumOfSensors(numSensors)
		.setInitialTimestamp(initialTime)
		.setBoundingCuboid(minValues, maxValues)
		.setTimestampUnitPerSecond(timeStampUnitsPerSecond)
		);
	
	// check initial sensor data
	std::set<sensorId_t> IdList;
	for (const auto& Sensor : Gen)
	{
		auto Pos = Sensor.position();

		// check for unique sensorId among generated sensors
		EXPECT_EQ(0, IdList.count(Sensor.sensorId()));
		IdList.emplace(Sensor.sensorId());

		// check for bounding box
		EXPECT_GE(Pos.x(), minValues.x());
		EXPECT_GE(Pos.y(), minValues.y());
		EXPECT_GE(Pos.z(), minValues.z());
		EXPECT_LE(Pos.x(), maxValues.x());
		EXPECT_LE(Pos.y(), maxValues.y());
		EXPECT_LE(Pos.z(), maxValues.z());

		EXPECT_EQ(Sensor.timestamp(), initialTime);
	}
	EXPECT_EQ(IdList.size(), numSensors);
}

TEST(Generator, Generation1sec)
{
	using namespace PositionGenerator;

	float maxVelocity = 10.0; // 10m/s
	uint64_t timeStampUnitsPerSecond = 1; // we use seconds now
	timestamp_t initialTime = 10; // we start at 10sec
	Vector3 minValues(10.f, 10.f, 0.2f);
	Vector3 maxValues(110.f, 110.f, 1.5f);
	int numSensors = 1;
	int numRounds = 10;

	Generator Gen(GenerationParameter()
		.setMaximalVelocity(maxVelocity)
		.setNumOfSensors(numSensors)
		.setInitialTimestamp(initialTime)
		.setBoundingCuboid(minValues, maxValues)
		.setTimestampUnitPerSecond(timeStampUnitsPerSecond)
	);

	// store initial sensor data for later comparison
	std::map<sensorId_t, SensorPosition> Data;
	for (const auto& Sensor : Gen)
	{
		Data.emplace(Sensor.sensorId(), Sensor);
	}
	EXPECT_EQ(Data.size(), numSensors);
	
	// now generate position for the next 1s
	auto testTime = initialTime;
	for (int round = 0; round < numRounds; ++round)
	{
		testTime += 1; // we set the generator to 1 unit per second

		Gen.generateData(testTime);
		for (const auto& Sensor : Gen)
		{
			SensorPosition& Old = Data[Sensor.sensorId()];
			Vector3 move = Sensor.position() - Old.position();
			// store new value for next round
			Old.setPosition(Sensor.position());

			// no movement in z direction
			EXPECT_EQ(move.z(), 0);

			float speed = sqrtf(scalarProduct(move, move)); // as we simulated with 1s speed equals velocity (m/s *s)
			EXPECT_LE(speed, maxVelocity);
			EXPECT_GT(speed, 0.f); 

			// check bounding box
			auto Pos = Sensor.position();
			EXPECT_GE(Pos.x(), minValues.x());
			EXPECT_GE(Pos.y(), minValues.y());
			EXPECT_GE(Pos.z(), minValues.z());
			EXPECT_LE(Pos.x(), maxValues.x());
			EXPECT_LE(Pos.y(), maxValues.y());
			EXPECT_LE(Pos.z(), maxValues.z());
		}
	}
}

TEST(Generator, DISABLED_GenerationMySec)
{
	using namespace PositionGenerator;

	float maxVelocity = 10.0; // 10m/s
	uint64_t timeStampUnitsPerSecond = 1000000; // usec
	timestamp_t initialTime = 10* timeStampUnitsPerSecond + 450000; // we start at 10,45sec
	Vector3 minValues(10.f, 10.f, 0.2f);
	Vector3 maxValues(110.f, 110.f, 1.5f);
	int numSensors = 120;

	Generator Gen(GenerationParameter()
		.setMaximalVelocity(maxVelocity)
		.setNumOfSensors(numSensors)
		.setInitialTimestamp(initialTime)
		.setBoundingCuboid(minValues, maxValues)
		.setTimestampUnitPerSecond(timeStampUnitsPerSecond)
	);

	// store initial sensor data for later comparison
	std::map<sensorId_t, SensorPosition> Data;
	for (const auto& Sensor : Gen)
	{
		Data.emplace(Sensor.sensorId(), Sensor);
	}
	EXPECT_EQ(Data.size(), numSensors);

	// now generate position for the next 1s
	auto testTime = initialTime;
	for (int round = 0; round < 100; ++round)
	{
		testTime += timeStampUnitsPerSecond; // we set the generator to 1 unit per second

		Gen.generateData(testTime);
		for (const auto& Sensor : Gen)
		{
			SensorPosition& Old = Data[Sensor.sensorId()];
			Vector3 move = Sensor.position() - Old.position();

			// no movement in z direction
			EXPECT_EQ(move.z(), 0);

			float speed = sqrtf(scalarProduct(move, move)); // as we simulated with 1s speed equals velocity (m/s *s)
			EXPECT_LE(speed, maxVelocity);

			// check bounding box
			auto Pos = Sensor.position();
			EXPECT_GE(Pos.x(), minValues.x());
			EXPECT_GE(Pos.y(), minValues.y());
			EXPECT_GE(Pos.z(), minValues.z());
			EXPECT_LE(Pos.x(), maxValues.x());
			EXPECT_LE(Pos.y(), maxValues.y());
			EXPECT_LE(Pos.z(), maxValues.z());

			// store new value for next round
			Old=Sensor;
		}
	}
}



