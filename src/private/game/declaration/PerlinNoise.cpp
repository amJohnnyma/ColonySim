#include "PerlinNoise.h"

PerlinNoise::PerlinNoise(unsigned int seed)
{
    p.resize(256); //permutation table init
    std::iota(p.begin(),p.end(),0); // fill 0...255

    std::default_random_engine engine(seed);
    std::shuffle(p.begin(),p.end(),engine);

    //duplicate perm vector
    p.insert(p.end(), p.begin(),p.end());
}

PerlinNoise::~PerlinNoise()
{
}

float PerlinNoise::noise(float x, float y)
{
    int xi = static_cast<int>(std::floor(x)) & 255;
    int yi = static_cast<int>(std::floor(y)) & 255;

    float xf = x - std::floor(x);
    float yf = y - std::floor(y);

    float u = fade(xf);
    float v = fade(yf);

    // Hash coordinates of the 4 square corners
    int aa = p[p[xi] + yi];
    int ab = p[p[xi] + yi + 1];
    int ba = p[p[xi + 1] + yi];
    int bb = p[p[xi + 1] + yi + 1];

    // Gradient and dot product at each corner
    float x1 = lerp(grad(aa, xf,     yf),     // Bottom-left
                    grad(ba, xf - 1, yf), u); // Bottom-right
    float x2 = lerp(grad(ab, xf,     yf - 1), // Top-left
                    grad(bb, xf - 1, yf - 1), u); // Top-right

    return (lerp(x1, x2, v) + 1.0f) / 2.0f; // Normalize to [0,1]




    return 0.0;
}
