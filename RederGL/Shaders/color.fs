#version 330 core
out vec4 FragColor;

in vec4 Coord;

// uniform vec4 color;

void main()
{

    
    FragColor = vec4(round(abs((Coord.x+0.5f) - floor(Coord.x))), round(abs((Coord.y+0.5f) - floor(Coord.y))), round(abs((Coord.z+0.5f) - floor(Coord.z))), 0.0f);
    // FragColor = vec4(sin(Coord.x * 3.14f), cos(Coord.y* 3.14f), sin(Coord.z*3.14f), 1.0f);
    // FragColor = vec4(exp(sin(Coord.x * 3.14f)), exp(cos(Coord.y* 3.14f)), exp(sin(Coord.z*3.14f)), 1.0f);
    // FragColor = vec4(0.0, 0.0,0.0, 1.0f);
    // if ( (abs(Coord.z - trunc(Coord.z)) < 0.05f)  || (abs(Coord.x - trunc(Coord.x)) < 0.05f)  )
    // {
    //     FragColor =vec4(1.0f, 1.0f,1.0f, 1.0f);
    // }
}

