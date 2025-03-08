#version 330 core
out vec4 FragColor;

in vec4 Coord;
flat in uint blockID;

// uniform vec4 color;

void main()
{
    float am = mod(abs(Coord.y-52.0f), 53.0f)/53.f+ 0.074f;

    
    if (blockID == 1u)
    {
        FragColor = vec4(0.0f,  1.f - am, 0.0f, 1.0f);
    }
    else if (blockID == 2u)
    {
        FragColor = vec4(1.0f,  0.0f, 0.0f, 0.5f);
    }
    else if (blockID == 0u)
    {
        FragColor = vec4(1.0f,  1.0f, 1.f, 1.0f);
    }
    else 
    {
        FragColor = vec4(0.0f,  0.0f, 0.f, 1.0f);
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

