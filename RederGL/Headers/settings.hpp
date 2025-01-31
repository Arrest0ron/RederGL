#ifndef SETTINGS_HPP
#define SETTINGS_HPP


const int CHUNKS = 4 * 4;
const unsigned DEPTH = 192;
const int CHUNK_SIZE = 16*16*DEPTH;
int DEBUG = false;
bool DEBUG_PRESS_HINT = true;
int SEED = time(0); 

#endif //SETTINGS_HPP