#ifndef CHUNK_HPP
#define CHUNK_HPP
#include "settings.hpp"
#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"
#include <cstring>
#include <string>

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
    int _x, _y;
    int blocks[16][16][DEPTH];
    BiomeType Biome; 
    public:
    Chunk(int x, int y)
    {


        std::memset(blocks, 0, sizeof(blocks)); // Zero out the array
        float lacunarity = 2.0f, gain = 0.3f;
        int octaves = 5;

        int SEED_X = (rand())%100003, SEED_Z = (rand())%100151;
        
        for (int x = 0; x!=16; x++)
        {
            for (int z = 0; z != 16; z++)
            {

      
            // int HASH = 489144;
            
            // Scale the coordinates and add an offset to avoid integer grid points
                float scale = 0.02f;
                int y = stb_perlin_fbm_noise3(((float)(x+SEED_X) )* scale, 0.0f, (float)(z+SEED_Z) * scale, lacunarity, gain, octaves)*10+(DEPTH/2);
                blocks[x][y][z] = 1;
            }
        }
    }
};



#endif //CHUNK_HPP