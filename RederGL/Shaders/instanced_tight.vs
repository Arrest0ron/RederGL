#version 330 core

// Vertex shader inputs
layout(location = 0) in vec3 aPos;         // Local block position (e.g., cube vertices)
layout(location = 1) in uint aOffset;      // Packed block offset (x, y, z)
// layout(location = 2) in vec2 chunkPosition;// Chunk coordinates (chunkX, chunkZ)

// Uniforms for view and projection matrices
uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;
uniform vec2 chunkPosition;

// Output to fragment shader
out vec4 Coord;
flat out uint blockID;

void main() {
    blockID = uint(int(aOffset) & 0xFFFF);        // Extract bits 0-15 (16 bits)
    // Unpack the block offset
    float x = float((int(aOffset) >> 28) & 15);   // Extract bits 28-31 (4 bits)
    float y = float((int(aOffset) >> 20) & 255);  // Extract bits 20-27 (8 bits)
    float z = float((int(aOffset) >> 16) & 15);   // Extract bits 16-19 (4 bits)
    
    

    // Calculate global position
    vec3 globalPos = vec3(
        x + chunkPosition.x * 16.0, // Global X
        y,                          // Global Y
        z + chunkPosition.y * 16.0  // Global Z
    );

    // Combine with local position
    Coord = vec4(aPos + globalPos, 1.0);

    // Transform to clip space
    gl_Position = projection * view * model * Coord;
}