#pragma once
#include <random>
#include <chrono>

namespace Core {


	struct Rng 
	{
		static std::mt19937_64 rng; 
		static int Randint(int l = -1000000000, int r = 1000000000);
		static float Randfloat(float = -1e9f, float r = 1e9f);
	};

}