#version 330 core
out vec4 FragColor;

in vec4 Coord;

uniform vec4 color;

void main()
{
    
    FragColor = vec4(exp(sin(Coord.x * 3.14f)), exp(cos(Coord.y* 3.14f)), exp(sin(Coord.z*3.14f)), 1.0f);
    // if ( (abs(Coord.z - trunc(Coord.z)) < 0.05f)  || (abs(Coord.x - trunc(Coord.x)) < 0.05f)  )
    // {
    //     FragColor =vec4(1.0f, 1.0f,1.0f, 1.0f);
    // }
}

