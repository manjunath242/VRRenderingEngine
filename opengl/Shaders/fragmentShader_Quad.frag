#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D screenTexture;

const float offset = 1.0 / 300;

void main()
{ 
    // Normal
	color = texture(screenTexture, TexCoords);

	// Simple Post Processing Effects
	// 1. Inversion
	//color = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);

	// 2. Grayscale
	//color = texture(screenTexture, TexCoords);
    //float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
    //color = vec4(average, average, average, 1.0);

	// 3. Kernel Effects
	// Sharpen Kernel
	vec2 offsets[9] = vec2[](
        vec2(-offset, offset),  // top-left
        vec2(0.0f,    offset),  // top-center
        vec2(offset,  offset),  // top-right
        vec2(-offset, 0.0f),    // center-left
        vec2(0.0f,    0.0f),    // center-center
        vec2(offset,  0.0f),    // center-right
        vec2(-offset, -offset), // bottom-left
        vec2(0.0f,    -offset), // bottom-center
        vec2(offset,  -offset)  // bottom-right    
    );

	// Define the kernel
	// Sharpen kernel that sharpens each color value by sampling all surrounding pixels in an interesting way
    /*float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );*/
	// Blur Kernel
	/*float kernel[9] = float[](
		1.0 / 16, 2.0 / 16, 1.0 / 16,
		2.0 / 16, 4.0 / 16, 2.0 / 16,
		1.0 / 16, 2.0 / 16, 1.0 / 16  
	);*/
	// Edge Detection
	/*float kernel[9] = float[](
        1, 1, 1,
        1, -8, 1,
        1, 1, 1
    );*/
    /*
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
    
    color = vec4(col, 1.0);
	*/
}