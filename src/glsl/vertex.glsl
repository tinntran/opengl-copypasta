#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vertexColor;

uniform vec3 dc;

void main(void)
{
  gl_Position = vec4(aPos.x, aPos.y, 0, 1.0);

  vertexColor = abs(aColor - dc);
}
