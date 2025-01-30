#version 330 core
out vec4 FragColor;

in vec3 Coord;

uniform vec4 color;

void main()
{
    
    FragColor = vec4(Coord, 1.0f);
    if ( (abs(Coord.z - trunc(Coord.z)) < 0.02f)  || (abs(Coord.x - trunc(Coord.x)) < 0.02f)  )
    {
        FragColor = vec4(0.f, 0.f, 0.f, 1.0f);
    }
}

