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
    if (x < 0 || x > 15 || y < 0 || y > 255 || z < 0 || z > 15 || blockID < 0 || blockID > 65535) {
        // Handle invalid input (e.g., throw an exception, return an error code, etc.)
        throw std::invalid_argument("Input values out of range");
    }
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
    uint32_t blocks[16][DEPTH][16];
    uint16_t bitmap[16][256];
    Chunk(int chunk_x, int chunk_z) : _x(chunk_x), _z(chunk_z)
    {   
        bitmap[16][256] = {0}; // 0 init

        for (int x = 0; x!=16; x++)
        {
            for (int z = 0; z != 16; z++)
            {
                float scale = 0.02f;
                
                int y = (stb_perlin_fbm_noise3(((float)(x+SEED_X+chunk_x*16) )* scale, 0.0f, (float)(z+SEED_Z+chunk_z*16) * scale, biome.lacunarity, biome.gain, biome.octaves)+ 1.f)*(20)+50;
                // if (y > 45)
                // {
                    // std::cout << "solid at " << x << " " << y << " " << z << std::endl;
                    blocks[x][y][z] = packBlockData(x,y,z,1);
                    set_solid_block(x, y, z);
                    
                // }
                // else 
                // {
                    for (int i =0; i != y; i++)
                    {
                        blocks[x][i][z] = packBlockData(x,i,z,2);
                        set_solid_block(x, i, z);
                    }
                // }
                
            }

        }
        evaluate_visibility();
    }
    void evaluate_visibility() {
        visible_blocks.clear(); // Clear previous visible blocks
    
        for (int x = 0; x != 16; x++) {
            for (int z = 0; z != 16; z++) {
                for (int y = 0; y != DEPTH; y++) {
                    // Check if the current block is solid
                    if (bitmap[x][y] & (1 << z)) {
                        bool isVisible = false;
    
                        // Check all 6 neighbors
                        // Left neighbor (x-1, y, z)
                        if (x == 0 || !(bitmap[x - 1][y] & (1 << z))) {
                            isVisible = true;
                        }
    
                        // Right neighbor (x+1, y, z)
                        if (!isVisible && (x == 15 || !(bitmap[x + 1][y] & (1 << z)))) {
                            isVisible = true;
                        }
    
                        // Down neighbor (x, y-1, z)
                        if (!isVisible && (y == 0 || !(bitmap[x][y - 1] & (1 << z)))) {
                            isVisible = true;
                        }
    
                        // Up neighbor (x, y+1, z)
                        if (!isVisible && (y == DEPTH - 1 || !(bitmap[x][y + 1] & (1 << z)))) {
                            isVisible = true;
                        }
    
                        // Back neighbor (x, y, z-1)
                        if (!isVisible && (z == 0 || !(bitmap[x][y] & (1 << (z - 1))))) {
                            isVisible = true;
                        }
    
                        // Front neighbor (x, y, z+1)
                        if (!isVisible && (z == 15 || !(bitmap[x][y] & (1 << (z + 1))))) {
                            isVisible = true;
                        }
    
                        // If the block is visible, add it to the visible_blocks vector
                        if (isVisible) {
                            visible_blocks.push_back(blocks[x][y][z]);
                        }
                    }
                }
            }
        }
        // std::cout << "EVALUATED " << visible_blocks.size() << " VISIBLE BLOCKS FOR CHUNK " << _x << ":" << _z << std::endl;
    }
    void update_bitmap(std::vector<std::vector<Chunk*>>& chunks)
    {
        
    }
    void set_solid_block(int x, int y, int z) {
        // Determine which layer (0–15) and bit position (0–15) within the layer
    
        // Ensure x, y, and z are within bounds
        if (x < 0 || x >= 16 || z < 0 || z >= 16 || y < 0 || y >= 256) {
            std::cout << "setsoliderr";
            return; // Out of bounds, do nothing
        }
    
        // Set the specific bit using bit shifting and bitwise OR
        // std::cout << x << " " <<<< std::endl;/
        bitmap[x][y] |= (1 << z);
    }
};

#endif //CHUNK_HPP