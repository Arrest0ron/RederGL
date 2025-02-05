#version 330 core
out vec4 FragColor;

in vec4 Coord;
flat in uint blockID;

// uniform vec4 color;

void main()
{
    float am = mod(abs(Coord.y-26.0f), 26.0f)/54.f+ 0.074f;

    
    if (blockID == 1u)
    {
        FragColor = vec4(0.0f,  1.f - am, 0.0f, 1.0f);
    }
    else
    {
        FragColor = vec4(0.0f,  0.0f, 1.f -am, 1.0f);
    }
    
    // FragColor = vec4(Coord.x, Coord.y, Coord.z, 1.0f);
    // FragColor = vec4(sin(Coord.x * 3.14f), cos(Coord.y* 3.14f), sin(Coord.z*3.14f), 1.0f);
    // FragColor = vec4(exp(sin(Coord.x * 3.14f)), exp(cos(Coord.y* 3.14f)), exp(sin(Coord.z*3.14f)), 1.0f);
    // FragColor = vec4(0.0, 0.0,0.0, 1.0f);
    // if ( (abs(Coord.z - trunc(Coord.z)) < 0.05f)  || (abs(Coord.x - trunc(Coord.x)) < 0.05f)  )
    // {
    //     FragColor =vec4(1.0f, 1.0f,1.0f, 1.0f);
    // }
}

