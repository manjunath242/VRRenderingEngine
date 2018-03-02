#version 330 core
out vec4 color;

uniform float mixValue;

void main()
{
    //color = vec4(1.0f); // Set all 4 vector values to 1.0f
	vec4 grayColor = vec4(0.41f, 0.41f, 0.41f, 1.0f);
	vec4 greenColor = vec4(0.11f,0.46f,0.14f,1.0f);
	color = mix(grayColor, greenColor, mixValue);
}