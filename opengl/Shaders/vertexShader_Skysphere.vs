#version 330 core
layout (location = 0) in vec3 position;

out vec3 ourPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	// Note that we read the multiplication from right to left
    vec4 pos = projection * view * model * vec4(position, 1.0f);
	gl_Position = pos.xyww;
	ourPosition = position;
}