#version 330 core
layout (location = 0) in vec4 vertex;
//layout (location = 0) in vec4 in_position;
//layout (location = 1) in vec4 in_velocity;
//layout (location = 2) in vec4 in_color;
//layout (location = 3) in float in_rotation;
//layout (location = 4) in float in_angular;
//layout (location = 5) in float in_age;

out vec2 TexCoord;
out vec4 particleColor;

//out vec4 vertOutColor;
//out float vertOutRotation;
//out float vertOutAge;

uniform mat4 view;
uniform mat4 projection;
uniform vec4 ourColor;
uniform vec3 position;
uniform mat4 model;
uniform float scale;


void main()
{
	vec3 actualPosition=vec3(vertex.xy*scale,0)+position;
	TexCoord = vertex.zw;
	//mat4 transposeMat = transpose(view);
	vec4 position = vec4(actualPosition, 1.0f);
	particleColor=ourColor;
	mat4 viewProj = projection*view;
	mat4 viewProjTran = transpose(viewProj);
	gl_Position = viewProj*position;
	
   



	//gl_Position=projection*view*in_position;

	//vertOutColor=in_color;
	//vertOutRotation=in_rotation;
	//vertOutAge=in_age;
    
    
}