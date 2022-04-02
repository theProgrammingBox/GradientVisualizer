#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include "Randoms.h"

using namespace std;
using namespace olc;

class Data
{
public:
	vector<float> w1g;
	vector<float> w2g;
	vector<float> w3g;
	vector<float> w4g;
	vector<float> w5g;
	vector<float> w6g;

	vector<float> b1g;
	vector<float> b2g;
	vector<float> b3g;
	vector<float> b4g;

	vector<float> w1pg;
	vector<float> w2pg;
	vector<float> w3pg;
	vector<float> w4pg;
	vector<float> w5pg;
	vector<float> w6pg;

	vector<float> b1pg;
	vector<float> b2pg;
	vector<float> b3pg;
	vector<float> b4pg;

	vector<float> pgstd;

	int batchCount;
};

float Relu2(float x)
{
	return x < 0 ? x * 0.1 : x;
	return x < -1 ? x + 2 : (x < 1 ? -x : x - 2);
}

float ReluGradient2(float x)
{
	return x < 0 ? 0.1 : 1;
	return (x < -1) || (x > 1) ? 1 : -1;
}

class Example : public olc::PixelGameEngine
{
public:
	vector<Data> datas;
	int iterationIndex;

	Random random = Random();

	float expected;
	float input;
	float output;

	float gradient;

	float rGrad1;
	float rGrad2;
	float rGrad3;

	float w1 = random.DoubleRandom();
	float w2 = random.DoubleRandom();
	float w3 = random.DoubleRandom();
	float w4 = random.DoubleRandom();
	float w5 = random.DoubleRandom();
	float w6 = random.DoubleRandom();

	float b1 = random.DoubleRandom();
	float b2 = random.DoubleRandom();
	float b3 = random.DoubleRandom();
	float b4 = random.DoubleRandom();

	float w1g;
	float w2g;
	float w3g;
	float w4g;
	float w5g;
	float w6g;

	float b1g;
	float b2g;
	float b3g;
	float b4g;

	float w1pg;
	float w2pg;
	float w3pg;
	float w4pg;
	float w5pg;
	float w6pg;

	float b1pg;
	float b2pg;
	float b3pg;
	float b4pg;

	float pgstdArr[10]{};
	float pgstd;
	int pgstdIndex;
	int batchCount;

	float rate = 0.01;

	Example()
	{
		sAppName = "Visualizing";
	}

	bool OnUserCreate() override
	{
		iterationIndex = 0;

		for (int iteration = 0; iteration < 1000; iteration++)
		{
			Data data;

			batchCount = 0;
			pgstdIndex = 0;
			pgstd = 0;

			for (int i = 0; i < 10; i++)
			{
				pgstdArr[i] = 10;
			}

			w1pg = 0;
			w2pg = 0;
			w3pg = 0;
			w4pg = 0;
			w5pg = 0;
			w6pg = 0;

			b1pg = 0;
			b2pg = 0;
			b3pg = 0;
			b4pg = 0;

			while (batchCount < 10 || pgstd > 0.1)
			{
				input = random.DoubleRandom() * 10;
				expected = input;

				float in1 = w1 * input + b1;
				float in2 = w2 * input + b2;
				float in3 = w3 * input + b3;

				float r1 = Relu2(in1);
				float r2 = Relu2(in2);
				float r3 = Relu2(in3);

				output =
					w4 * r1 +
					w5 * r2 +
					w6 * r3 +
					b4;

				gradient = expected - output;

				b4g = gradient;

				w4g = r1 * gradient;
				w5g = r2 * gradient;
				w6g = r3 * gradient;

				rGrad1 = ReluGradient2(in1) * w4 * gradient;
				rGrad2 = ReluGradient2(in2) * w5 * gradient;
				rGrad3 = ReluGradient2(in3) * w6 * gradient;

				b1g = rGrad1;
				b2g = rGrad2;
				b3g = rGrad3;

				w1g = input * rGrad1;
				w2g = input * rGrad2;
				w3g = input * rGrad3;

				data.w1g.push_back(w1g);
				data.w2g.push_back(w2g);
				data.w3g.push_back(w3g);
				data.w4g.push_back(w4g);
				data.w5g.push_back(w5g);
				data.w6g.push_back(w6g);

				data.b1g.push_back(b1g);
				data.b2g.push_back(b2g);
				data.b3g.push_back(b3g);
				data.b4g.push_back(b4g);

				if (batchCount != 0)
				{
					pgstdArr[pgstdIndex] =
						sqrt(
							pow(((w1pg + w1g) / (batchCount + 1)) - (w1pg / batchCount), 2) +
							pow(((w2pg + w2g) / (batchCount + 1)) - (w2pg / batchCount), 2) +
							pow(((w3pg + w3g) / (batchCount + 1)) - (w3pg / batchCount), 2) +
							pow(((w4pg + w4g) / (batchCount + 1)) - (w4pg / batchCount), 2) +
							pow(((w5pg + w5g) / (batchCount + 1)) - (w5pg / batchCount), 2) +
							pow(((w6pg + w6g) / (batchCount + 1)) - (w6pg / batchCount), 2) +
							pow(((b1pg + b1g) / (batchCount + 1)) - (b1pg / batchCount), 2) +
							pow(((b2pg + b2g) / (batchCount + 1)) - (b2pg / batchCount), 2) +
							pow(((b3pg + b3g) / (batchCount + 1)) - (b3pg / batchCount), 2) +
							pow(((b4pg + b4g) / (batchCount + 1)) - (b4pg / batchCount), 2)
						);
				}
				else
				{
					pgstdArr[pgstdIndex] =
						sqrt(
							pow(w1g, 2) +
							pow(w2g, 2) +
							pow(w3g, 2) +
							pow(w4g, 2) +
							pow(w5g, 2) +
							pow(w6g, 2) +
							pow(b1g, 2) +
							pow(b2g, 2) +
							pow(b3g, 2) +
							pow(b4g, 2)
						);
				}

				pgstdIndex = pgstdIndex + 1 == 10 ? 0 : pgstdIndex + 1;

				pgstd = 0;
				for (int i = 0; i < min(batchCount, 10); i++)
				{
					pgstd += pgstdArr[i];
				}
				pgstd /= min(batchCount, 10);

				data.pgstd.push_back(pgstd);

				w1pg += w1g;
				w2pg += w2g;
				w3pg += w3g;
				w4pg += w4g;
				w5pg += w5g;
				w6pg += w6g;

				b1pg += b1g;
				b2pg += b2g;
				b3pg += b3g;
				b4pg += b4g;

				data.w1pg.push_back(w1pg / batchCount);
				data.w2pg.push_back(w2pg / batchCount);
				data.w3pg.push_back(w3pg / batchCount);
				data.w4pg.push_back(w4pg / batchCount);
				data.w5pg.push_back(w5pg / batchCount);
				data.w6pg.push_back(w6pg / batchCount);

				data.b1pg.push_back(b1pg / batchCount);
				data.b2pg.push_back(b2pg / batchCount);
				data.b3pg.push_back(b3pg / batchCount);
				data.b4pg.push_back(b4pg / batchCount);

				batchCount++;
			}

			data.batchCount = batchCount;

			w1 += rate * w1pg / batchCount;
			w2 += rate * w2pg / batchCount;
			w3 += rate * w3pg / batchCount;
			w4 += rate * w4pg / batchCount;
			w5 += rate * w5pg / batchCount;
			w6 += rate * w6pg / batchCount;

			b1 += rate * b1pg / batchCount;
			b2 += rate * b2pg / batchCount;
			b3 += rate * b3pg / batchCount;
			b4 += rate * b4pg / batchCount;

			datas.push_back(data);

			/*fout
				<< setw(10) << "batchCount"
				<< setw(10) << "w1pg"
				<< setw(10) << "w2pg"
				<< setw(10) << "w3pg"
				<< setw(10) << "w4pg"
				<< setw(10) << "w5pg"
				<< setw(10) << "w6pg"
				<< setw(10) << "b1pg"
				<< setw(10) << "b2pg"
				<< setw(10) << "b3pg"
				<< setw(10) << "b4pg"
				<< endl;*/

				/*fout
					<< setw(10) << batchCount
					<< setw(10) << w1pg / batchCount
					<< setw(10) << w2pg / batchCount
					<< setw(10) << w3pg / batchCount
					<< setw(10) << w4pg / batchCount
					<< setw(10) << w5pg / batchCount
					<< setw(10) << w6pg / batchCount
					<< setw(10) << b1pg / batchCount
					<< setw(10) << b2pg / batchCount
					<< setw(10) << b3pg / batchCount
					<< setw(10) << b4pg / batchCount
					<< "\n\n";*/
		}

		cout
			<< setw(10) << "w1"
			<< setw(10) << "w2"
			<< setw(10) << "w3"
			<< setw(10) << "w4"
			<< setw(10) << "w5"
			<< setw(10) << "w6"
			<< setw(10) << "b1"
			<< setw(10) << "b2"
			<< setw(10) << "b3"
			<< setw(10) << "b4"
			<< endl;

		cout
			<< setw(10) << w1
			<< setw(10) << w2
			<< setw(10) << w3
			<< setw(10) << w4
			<< setw(10) << w5
			<< setw(10) << w6
			<< setw(10) << b1
			<< setw(10) << b2
			<< setw(10) << b3
			<< setw(10) << b4
			<< "\n\n";

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		DrawLine(vi2d(0, 500), vi2d(1000, 500), BLACK);

		for (int iteration = 1; iteration < datas[iterationIndex].batchCount; iteration++)
		{
			DrawLine(
				vi2d(
					(iteration - 1) * (1000.0 / datas[iterationIndex].batchCount),
					500 - datas[iterationIndex].pgstd[iteration - 1] * 10),
				vi2d(
					iteration * (1000.0 / datas[iterationIndex].batchCount),
					500 - datas[iterationIndex].pgstd[iteration] * 10),
				BLACK);
			FillCircle(
				vi2d(
					iteration * (1000.0 / datas[iterationIndex].batchCount),
					500 - datas[iterationIndex].w1g[iteration] * 10),
				500.0 / datas[iterationIndex].batchCount,
				BLACK);
			/*DrawLine(
				vi2d(
					(iteration - 1) * (1000.0 / datas[iterationIndex].batchCount),
					500 - datas[iterationIndex].w1pg[iteration - 1] * 10),
				vi2d(
					iteration * (1000.0 / datas[iterationIndex].batchCount),
					500 - datas[iterationIndex].w1pg[iteration] * 10),
				BLACK);*/
		}

		if (GetKey(olc::Key::UP).bPressed) iterationIndex + 1 == datas.size() ? false : iterationIndex++;
		if (GetKey(olc::Key::DOWN).bPressed) iterationIndex - 1 == -1 ? false : iterationIndex--;

		DrawLine(vi2d(0, 500), vi2d(1000, 500), WHITE);

		for (int iteration = 1; iteration < datas[iterationIndex].batchCount; iteration++)
		{
			DrawLine(
				vi2d(
					(iteration - 1) * (1000.0 / datas[iterationIndex].batchCount),
					500 - datas[iterationIndex].pgstd[iteration - 1] * 10),
				vi2d(
					iteration * (1000.0 / datas[iterationIndex].batchCount),
					500 - datas[iterationIndex].pgstd[iteration] * 10),
				WHITE);
			FillCircle(
				vi2d(
					iteration * (1000.0 / datas[iterationIndex].batchCount),
					500 - datas[iterationIndex].w1g[iteration] * 10),
				250.0 / datas[iterationIndex].batchCount,
				GREEN);
			/*DrawLine(
				vi2d(
					(iteration - 1) * (1000.0 / datas[iterationIndex].batchCount),
					500 - datas[iterationIndex].w1pg[iteration - 1] * 10),
				vi2d(
					iteration * (1000.0 / datas[iterationIndex].batchCount),
					500 - datas[iterationIndex].w1pg[iteration] * 10),
				GREY);*/
		}

		return true;
	}
};

int main()
{
	Example demo;
	if (demo.Construct(1000, 1000, 1, 1))
		demo.Start();

	return 0;
}