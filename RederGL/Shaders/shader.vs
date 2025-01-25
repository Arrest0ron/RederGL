#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aColor; // the color variable has attribute position 1
  
out vec4 ourColor; // output a color to the fragment shader
uniform float offsetX;
uniform float offsetY;
void main()
{
    // vec4 Middle = (aPos.x, aPos.y,aPos.z, 1.0);
    gl_Position =  vec4(aPos.x + offsetX, aPos.y+offsetY,aPos.z, 1.0);
    ourColor = gl_Position; // set ourColor to the input color we got from the vertex data
}   