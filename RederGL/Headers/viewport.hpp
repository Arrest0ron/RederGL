
#ifndef VIEWPORT_HPP
#define VIEWPORT_HPP

#include <vector>
#include <chunk.hpp>

const int CHUNK_ROWS = 64;
const int CHUNK_COLS = 64;

class Viewport
{
public:
    std::vector<std::vector<Chunk*>>chunks;
    int chunk_map[CHUNK_ROWS][CHUNK_COLS];
    Viewport()
    {
        chunks = std::vector<std::vector<Chunk*>>(CHUNK_ROWS, std::vector<Chunk*>(CHUNK_COLS));
        for (int x = 0; x!=CHUNK_ROWS; x++)
        {
            for (int z = 0; z != CHUNK_COLS; z++)
            {
                chunks[x][z] = new Chunk(x,z);
            }
        }
        update_chunks();
    }
    void update_chunks()
    {
        for (int chunk_x = 0; chunk_x!= CHUNK_ROWS; chunk_x++)
        {
            for (int chunk_z = 0; chunk_z!= CHUNK_COLS; chunk_z++)
            {
                chunks[chunk_x][chunk_z]->evaluate_visibility(chunks);
            }
        }
    }
};
  
#endif