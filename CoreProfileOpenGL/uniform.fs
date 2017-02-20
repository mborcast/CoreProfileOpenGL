#version 330 core
out vec4 color;
  
uniform float time; // We set this variable in the OpenGL code.

void main()
{
    color = vec4(0.0, (sin(time) * 0.5 + 0.5), 0.0, 1.0);
}  