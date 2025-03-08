#ifndef CHUNK_HPP
#define CHUNK_HPP
#include "settings.hpp"
#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"
#include <cstring>
#include <string>
#include <vector>
#include <cstdint>
#include <viewport.hpp>
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
BiomeType PLAINS_LOW("PlainsLow", 1.1f, 0.3f, 5);
BiomeType STRANGE("Strange", 7771.5f, 67000.f, 2);


class Chunk
{
    int _x, _z;
    BiomeType biome = PLAINS;
    public:
    std::vector<uint32_t> visible_blocks;
    std::vector<uint32_t> transparent_blocks;
    uint32_t blocks[16][DEPTH][16];
    uint16_t bitmap[16][DEPTH];

    Chunk(int chunk_x, int chunk_z) : _x(chunk_x), _z(chunk_z)
    {   
        BiomeType biomes[2] = {PLAINS, PLAINS_LOW};
        memset(blocks, 0, (16*DEPTH*16)*sizeof(uint32_t));
        biomes[0] = PLAINS;
        biomes[1] = PLAINS_LOW;
        bitmap[16][256] = {0}; // 0 init
        for (int x = 0; x!=16; x++)
        {
            for (int z = 0; z != 16; z++)
            {
                float scale = 0.02f;
                int y = (stb_perlin_fbm_noise3(((float)(x+SEED_X+chunk_x*16) )* scale, 0.0f, (float)(z+SEED_Z+chunk_z*16) * scale, biome.lacunarity, biome.gain, biome.octaves)+ 1.f)*(20)+50;
                blocks[x][y][z] = packBlockData(x,y,z,1);
                set_solid_block(x, y, z);
                int water = 67;
                while (water > y)
                {
                    blocks[x][water][z] = packBlockData(x,water,z,2);
                    // set_solid_block(x, water, z);
                    water--;
                }
                
                for (int i =0; i != y; i++)
                {
                    blocks[x][i][z] = packBlockData(x,i,z,1);
                    set_solid_block(x, i, z);
                }
                
            }

        }
    }
    void evaluate_visibility(const std::vector<std::vector<Chunk*>>& chunks) {
        visible_blocks.clear(); // Clear previous visible blocks
    
        for (int x = 0; x != 16; x++) {
            for (int z = 0; z != 16; z++) {
                for (int y = 0; y != DEPTH; y++) {
                    if (!blocks[x][y][z])
                    {
                        continue;
                    }
                    
                    // Check if the current block is solid
                    if (bitmap[x][y] & (1 << z)) {
                        bool isVisible = false;
    
                        // Check all 6 neighbors
                        // Left neighbor (x-1, y, z)
                        if (x == 0) {
                            // Border case: left edge of the chunk grid
                            if (_x == 0) {
                                // No chunk to the left, so the block is visible
                                isVisible = true;
                            } else {
                                // Check the bitmap of the neighboring chunk to the left
                                Chunk* leftChunk = chunks[_x - 1][_z];
                                if (!(leftChunk->bitmap[15][y] & (1 << z))) {
                                    isVisible = true;
                                }
                            }
                        } else if (!(bitmap[x - 1][y] & (1 << z))) {
                            isVisible = true;
                        }
    
                        // Right neighbor (x+1, y, z)
                        if (!isVisible && x == 15) {
                            // Border case: right edge of the chunk grid
                            if (_x == chunks.size() - 1) {
                                // No chunk to the right, so the block is visible
                                isVisible = true;
                            } else {
                                // Check the bitmap of the neighboring chunk to the right
                                Chunk* rightChunk = chunks[_x + 1][_z];
                                if (!(rightChunk->bitmap[0][y] & (1 << z))) {
                                    isVisible = true;
                                }
                            }
                        } else if (!isVisible && !(bitmap[x + 1][y] & (1 << z))) {
                            isVisible = true;
                        }
    
                        // Down neighbor (x, y-1, z)
                        if (!isVisible && y == 0) {
                            // Border case: bottom edge of the chunk
                            isVisible = true;
                        } else if (!isVisible && !(bitmap[x][y - 1] & (1 << z))) {
                            isVisible = true;
                        }
    
                        // Up neighbor (x, y+1, z)
                        if (!isVisible && y == DEPTH - 1) {
                            // Border case: top edge of the chunk
                            isVisible = true;
                        } else if (!isVisible && !(bitmap[x][y + 1] & (1 << z))) {
                            isVisible = true;
                        }
    
                        // Back neighbor (x, y, z-1)
                        if (!isVisible && z == 0) {
                            // Border case: back edge of the chunk grid
                            if (_z == 0) {
                                // No chunk behind, so the block is visible
                                isVisible = true;
                            } else {
                                // Check the bitmap of the neighboring chunk behind
                                Chunk* backChunk = chunks[_x][_z - 1];
                                if (!(backChunk->bitmap[x][y] & (1 << 15))) {
                                    isVisible = true;
                                }
                            }
                        } else if (!isVisible && !(bitmap[x][y] & (1 << (z - 1)))) {
                            isVisible = true;
                        }
    
                        // Front neighbor (x, y, z+1)
                        if (!isVisible && z == 15) {
                            // Border case: front edge of the chunk grid
                            if (_z == chunks[0].size() - 1) {
                                // No chunk in front, so the block is visible
                                isVisible = true;
                            } else {
                                // Check the bitmap of the neighboring chunk in front
                                Chunk* frontChunk = chunks[_x][_z + 1];
                                if (!(frontChunk->bitmap[x][y] & (1 << 0))) {
                                    isVisible = true;
                                }
                            }
                        } else if (!isVisible && !(bitmap[x][y] & (1 << (z + 1)))) {
                            isVisible = true;
                        }
    
                        // If the block is visible, add it to the visible_blocks vector
                        if (isVisible) {
                            visible_blocks.push_back(blocks[x][y][z]);
                        }
                    }
                    else 
                    {
                        transparent_blocks.push_back(blocks[x][y][z]);
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