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
	EXPECT_LT(v3.distanceSquared(v3), 1.E-10f);
	EXPECT_TRUE(isEqual(v1.distance(v2), sqrtf(2)));
	EXPECT_TRUE(isEqual(v1.distanceSquared(v2), 2.f));
}

TEST(Vector3, assignment)
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