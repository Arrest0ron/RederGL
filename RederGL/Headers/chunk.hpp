#ifndef CHUNK_HPP
#define CHUNK_HPP
#include "settings.hpp"
#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"
#include <cstring>
#include <string>
#include <vector>
#include <cstdint>
#include <iostream>

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

BiomeType PLAINS("Plains", 2.0f, 0.3f, 5);
BiomeType STRANGE("Strange", 7771.5f, 67000.f, 2);

class Chunk
{
    int _x, _z;
    
    BiomeType biome = PLAINS; 
    public:
    // int visible_blocks[16][DEPTH][16];
    std::vector<uint32_t> visible_blocks;
    std::vector<uint32_t> blocks;
    Chunk(int chunk_x, int chunk_z) : _x(chunk_x), _z(chunk_z)
    {

        // std::memset(visible_blocks.data(), packBlockData(x,y,z,1), 100*sizeof(uint32_t));
        for (int x = 0; x!=16; x++)
        {
            for (int z = 0; z != 16; z++)
            {
                float scale = 0.02f;
                int y = (stb_perlin_fbm_noise3(((float)(x+SEED_X+chunk_x*16) )* scale, 0.0f, (float)(z+SEED_Z+chunk_z*16) * scale, biome.lacunarity, biome.gain, biome.octaves)+ 1.f)*(26)+200;
                // if (y > 45)
                // {
                    blocks.push_back(packBlockData(x,y,z,2));
                // }
                // else 
                // {
                    for (int i =0; i != y; i++)
                    {
                        blocks.push_back(packBlockData(x,i,z,1));
                    }
                // }
                
            }

        }
        evaluate_visibility();
    }
    void evaluate_visibility()
    {
        for (uint32_t block : blocks)
        {
            if ((block & 0xFFFF) != 1)
            {
                // std::cout << (block & 0xFFFF) << " " << (block & 0xFFFF != 1) << std::endl;
                visible_blocks.push_back(block);
            }
        }
    }
};

#endif //CHUNK_HPP