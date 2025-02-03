#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aOffset;

uniform mat4 view;
uniform mat4 projection;

out vec4 Coord;

void main()
{
    gl_Position =  projection * view * vec4(aPos+aOffset, 1.0);
    Coord = vec4(aPos+aOffset, 1.0);
}  