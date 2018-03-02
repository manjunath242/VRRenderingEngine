#version 330 core
in vec3 ourPosition;

uniform float iGlobalTime;
uniform float isColor;
uniform float fog;

out vec4 color;

#define PI 3.14159265359
#define TWO_PI 6.28318530718

float random (in vec2 _st) { 
    return fract(sin(dot(_st.xy, vec2(12.9898,78.233))) * 43758.54531237);
}

// Based on Morgan McGuire @morgan3d
// https://www.shadertoy.com/view/4dS3Wd
float noise (in vec2 _st) {
    vec2 i = floor(_st);
    vec2 f = fract(_st);

    // Four corners in 2D of a tile
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    vec2 u = f * f * (3. - 2.0 * f);

    return mix(a, b, u.x) + 
            (c - a)* u.y * (1. - u.x) + 
            (d - b) * u.x * u.y;
}

#define NUM_OCTAVES 10

float fbm ( in vec2 _st) {
    float v = 0.0;
    float a = 0.4;
    vec2 shift = vec2(20.0);
    // Rotate to reduce axial bias
    mat2 rot = mat2(cos(0.5), sin(0.5), 
                    -sin(0.5), cos(0.50));
    for (int i = 0; i < NUM_OCTAVES; ++i) {
        v += a * noise(_st);
        _st = rot * _st * 2.2 + shift;
        a *= 0.5;
    }
    return v;
}

void main() {
	if(fog == 0.0)
	{
		vec2 st = (vec2(ourPosition) )/min(800,600);
		st *= 3.5;
    
		vec3 color1 = vec3(0.);
		vec2 a = vec2(0.);
		vec2 b = vec2(0.);
		vec2 c = vec2(600.,800.);
    
		a.x = fbm( st);
		a.y = fbm( st + vec2(1.0));
    
		b.x = fbm( st + 4.*a);
		b.y = fbm( st);

		c.x = fbm( st + 7.0*b + vec2(10.7,.2)+ 0.1215*iGlobalTime );
		c.y = fbm( st + 3.944*b + vec2(.3,12.8)+ 0.016*iGlobalTime);

		float f = fbm(st+b+c);

		//color1 = mix(vec3(0.445,0.002,0.419), vec3(1.000,0.467,0.174), clamp((f*f),0.2, 1.0));
		//color1 = mix(color1, vec3(0.413,0.524,0.880), clamp(length(c.x),0.480, 0.92));
		if(isColor == 0.0) {
			color1 = mix(vec3(0.502,0.502,0.502), vec3(0.25,0.25,0.25), clamp((f*f),0.2, 1.0));
			color1 = mix(color1, vec3(0.125,0.125,0.125), clamp(length(c.x),0.480, 0.92));
		} else if(isColor == 1.0) {
			color1 = mix(vec3(0.445,0.002,0.419), vec3(1.000,0.467,0.174), clamp((f*f),0.2, 1.0));
			color1 = mix(color1, vec3(0.413,0.524,0.880), clamp(length(c.x),0.480, 0.92));
		}
    
		st = st/3.5;

		vec3 finalColor = vec3(f*1.9*color1);
		color = vec4(finalColor*1.0, 1.0);
	}
	else if(fog == 0.01)
	{
		vec3 finalColor = vec3(0.5, 0.5, 0.5);
		color = vec4(finalColor*1.0, 1.0);
	}

}