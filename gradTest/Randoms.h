#pragma once

#include <chrono>

class Random
{
private:
	uint64_t seed;

public:
	Random()
	{
		seed = GenerateSeed();
	}

	uint64_t GenerateSeed()
	{
		uint64_t temp =
			std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() ^
			std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() ^
			std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() ^
			std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
		temp += UINT64_C(0x9E3779B97F4A7C15);
		temp = (temp ^ (temp >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
		temp = (temp ^ (temp >> 27)) * UINT64_C(0x94D049BB133111EB);
		return temp ^ (temp >> 31);
	}

	void Seed(long Seed)
	{
		seed = Seed;
	}

	uint64_t ULongRandom()
	{
		seed += UINT64_C(0x9E3779B97F4A7C15);
		seed = (seed ^ (seed >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
		seed = (seed ^ (seed >> 27)) * UINT64_C(0x94D049BB133111EB);
		return seed ^ (seed >> 31);

		return seed;
	}

	int64_t LongRandom()
	{
		return ULongRandom();
	}

	uint32_t UIntRandom()
	{
		return ULongRandom();
	}

	int32_t IntRandom()
	{
		return UIntRandom();
	}

	double UDoubleRandom()	// 0 through 1
	{
		return ULongRandom() * 5.42101086243e-20;
	}

	double DoubleRandom()	// -1 through 1
	{
		return LongRandom() * 1.08420217249e-19;
	}

	float UFloatRandom()	// 0 through 1
	{
		return UDoubleRandom();
	}

	float FloatRandom()	// -1 through 1
	{
		return DoubleRandom();
	}

	const double NormalRandom(double mean, double standardDeviation)
	{
		double x, y, radius;
		do
		{
			x = DoubleRandom();
			y = DoubleRandom();

			radius = x * x + y * y;
		} while (radius >= 1.0);

		return x * sqrt(-2.0 * log(radius) / radius) * standardDeviation + mean;
	}
};