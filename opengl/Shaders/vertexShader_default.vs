#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
	vec4 ViewSpace;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	// Note that we read the multiplication from right to left
    gl_Position = projection * view * model * vec4(position, 1.0f);
	vs_out.FragPos = vec3(model * vec4(position, 1.0f));
	vs_out.Normal = mat3(transpose(inverse(model))) * normal;
	vs_out.TexCoords = texCoords;
	vs_out.ViewSpace = view * model * vec4(position, 1.0f);
}