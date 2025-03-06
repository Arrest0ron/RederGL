
#ifndef VIEWPORT_HPP
#define VIEWPORT_HPP

#include <vector>
#include <chunk.hpp>

const int CHUNK_ROWS = 1;
const int CHUNK_COLS = 1;

class Viewport
{
public:
    std::vector<std::vector<Chunk*>>chunks;
    int chunk_map[CHUNK_ROWS][CHUNK_COLS];
    Viewport()
    {
    
    }
    void update_chunks()
    {
        for (int chunk_x = 0; chunk_x!= CHUNK_ROWS; chunk_x++)
        {
            for (int chunk_z = 0; chunk_z!= CHUNK_COLS; chunk_z++)
            {
                chunks[chunk_x][chunk_z]->update_bitmap(chunks);
            }
        }
    }
};
  
#endif