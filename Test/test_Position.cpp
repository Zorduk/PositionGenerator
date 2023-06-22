#include <limits>
#include <random>

#include "gtest/gtest.h"
#include "Position.h"

namespace 
{ 
	constexpr float threshold = 1.E-10f;
	bool isEqual(float First, float Second)
	{
		bool bEqual = false;
		if (fabs(First - Second) < threshold)
			bEqual = true;
		return bEqual;
	}

	bool isEqual(const PositionGenerator::Vector3& First, const PositionGenerator::Vector3& Second)
	{
		bool bEqual = false;
		if (abs(First.x() - Second.x() < threshold)
			&& abs(First.x() - Second.x() < threshold)
			&& abs(First.x() - Second.x() < threshold))
		{
			bEqual = true;
		}
		return bEqual;
	}
}

TEST(Vector3, generalOperation) 
{
	using namespace PositionGenerator;
	Vector3 v1(1.f, 0.f, 0.f);
	Vector3 v2(0.f, 1.f, 0.f);
	EXPECT_NE(v1, v2);

	auto v3 = v1 + v2;
	EXPECT_TRUE(isEqual(v3, Vector3(1.f, 1.f, 0.f)));
	EXPECT_LT(scalarProduct(v3-v3,v3-v3), 1.E-10f);
	EXPECT_TRUE(isEqual(scalarProduct(v2 - v1, v2 - v1), 2.f));
}

TEST(Vector3, assignmentAndCopy)
{
	using namespace PositionGenerator;
	std::random_device rd;
	std::mt19937 e2(rd());
	std::uniform_real_distribution<float> dist(std::numeric_limits<float>::min(),std::numeric_limits<float>::max());

	constexpr int numOfRuns = 10;
	for (int i = 0; i < numOfRuns; ++i)
	{
		Vector3 v1(dist(e2), dist(e2), dist(e2));
		Vector3 v2(v1);
		EXPECT_TRUE(isEqual(v1, v2)) << "v1=Vector3(" << v1.x() << "," << v1.y() << "," << v1.z() << ")";
		Vector3 v3 = v1;
		EXPECT_TRUE(isEqual(v1, v3)) << "v1=Vector3(" << v1.x() << "," << v1.y() << "," << v1.z() << ")";
	}
}

TEST(SensorPosition, dataStorage)
{
	using namespace PositionGenerator;
	std::random_device rd;
	std::mt19937 e2(rd());
	std::uniform_real_distribution<float> fDist(std::numeric_limits<float>::min(), std::numeric_limits<float>::max());
	std::uniform_int_distribution<uint64_t> iDist(std::numeric_limits<uint64_t>::min(), std::numeric_limits<uint64_t>::max());

	auto sensorId = iDist(e2);
	auto timestamp = iDist(e2);
	auto Pos = Vector3{ fDist(e2), fDist(e2), fDist(e2) };

	// initialising ctor
	SensorPosition Sensor1{sensorId, timestamp, Pos};
	EXPECT_EQ(sensorId, Sensor1.sensorId());
	EXPECT_EQ(timestamp, Sensor1.timestamp());
	EXPECT_TRUE(isEqual(Sensor1.position(), Pos));

	// assignment
	SensorPosition Sensor2;
	Sensor2.setSensorId(sensorId);
	Sensor2.setTimestamp(timestamp);
	Sensor2.setPosition(Pos);

	EXPECT_EQ(sensorId, Sensor2.sensorId());
	EXPECT_EQ(timestamp, Sensor2.timestamp());
	EXPECT_TRUE(isEqual(Sensor2.position(), Pos));
}