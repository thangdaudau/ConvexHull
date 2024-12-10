#include "Rng.h"

namespace Core {
    std::mt19937_64 Rng::rng((unsigned int)std::chrono::steady_clock::now().time_since_epoch().count());

    int Rng::Randint(int l, int r)
    {
        return std::uniform_int_distribution<>(l, r)(rng);
    }

    float Rng::Randfloat(float l, float r)
    {
        return std::uniform_real_distribution<float>(l, r)(rng);
    }

}
