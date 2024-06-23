#version 330 core

varying vec3 point_color;

void main()
{
	float col = point_color.z;	
	gl_FragColor = vec4(col, col, col, 1.0);
}
