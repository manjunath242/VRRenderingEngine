#version 330 core

in vec2 TexCoord;
in vec4 particleColor;

//out vec4 color;

//in vec2 uv;
in vec4 color;

uniform sampler2D ourTexture;

void main()
{
	//vec3 particle=vec3(texture(ourTexture, TexCoord));
	vec4 texColor=(texture(ourTexture, TexCoord));
	if(texColor.a < 0.1)
		discard;
   gl_FragColor =texColor*particleColor;

   //gl_FragColor = color;



}