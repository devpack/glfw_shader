#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 mvp;

varying vec3 point_color;

void main()
{
	gl_Position = mvp * vec4(position, 1.0);
	point_color = vec3(1.0, 1.0, 1.0);
}
