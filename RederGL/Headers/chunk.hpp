#ifndef CHUNK_HPP
#define CHUNK_HPP
#include "settings.hpp"
#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"
#include <cstring>
#include <string>
#include <vector>
#include <cstdint>

uint32_t packBlockData(int x, int y, int z, int blockID) {
    return (static_cast<uint32_t>(x) << 28) | // X uses bits 28-31 (4 bits)
           (static_cast<uint32_t>(y) << 20) | // Y uses bits 20-27 (8 bits)
           (static_cast<uint32_t>(z) << 16) | // Z uses bits 16-19 (4 bits)
           (static_cast<uint32_t>(blockID));  // Block ID uses bits 0-15 (16 bits)
}

struct BiomeType
{
    std::string name;
    float lacunarity, gain;
    int octaves;
    BiomeType(const std::string& BiomeName, float BiomeLacunarity,float BiomeGain,int BiomeOctaves) : name(BiomeName), lacunarity(BiomeLacunarity), gain(BiomeGain), octaves(BiomeOctaves) {}
};

BiomeType PLAINS("Plains", 2.0f, 0.3f, 6);


class Chunk
{
    int _x, _z;
    
    BiomeType biome = PLAINS; 
    public:
    // int blocks[16][DEPTH][16];
    std::vector<uint32_t> blocks;
    Chunk(int chunk_x, int chunk_z) : _x(chunk_x), _z(chunk_z)
    {
        float lacunarity = 2.0f, gain = 0.3f;
        int octaves = 5;
        for (int x = 0; x!=16; x++)
        {
            for (int z = 0; z != 16; z++)
            {
                float scale = 0.02f;
                int y = stb_perlin_fbm_noise3(((float)(x+SEED_X+chunk_x*16) )* scale, 0.0f, (float)(z+SEED_Z+chunk_z*16) * scale, lacunarity, gain, octaves)*10+20;
                blocks.push_back(packBlockData(x,y,z,1));
            }
        }
    }
};

#endif //CHUNK_HPP