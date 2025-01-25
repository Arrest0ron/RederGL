
#version 330 core
out vec4 FragColor;  
in vec4 ourColor;
  
void main()
{
    FragColor = vec4(ourColor.x*2+0.3f, ourColor.y*2+0.3f,ourColor.z*2+0.3f,ourColor.w*2+0.3f);
}