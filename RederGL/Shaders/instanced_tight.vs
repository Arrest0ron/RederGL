#version 330 core

// Vertex shader inputs
layout (location = 0) in vec3 aPos;
layout (location = 1) in uint aOffset;
layout (location = 2) in vec2 chunkPosition;

// Uniforms for view and projection matrices
uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

// Output to fragment shader
out vec4 Coord;

void main()
{
    float x =  float((int(aOffset) >> 28) & 15);
    float y =  float((int(aOffset) >> 20) & 255);
    float z = float((int(aOffset) >> 16) & 0xF);
    // uint blockID = aOffset & 0xFFFF;
    
    // int blockID = 4;
    // Unpack the offset and add it to the vertex position
    Coord = vec4(aPos + vec3(x+chunkPosition.x*16.0f,y,z+chunkPosition.y*16.0f), 1.0);
    gl_Position = projection * view * model*Coord;

    // Pass the combined position to the fragment shader
    
}