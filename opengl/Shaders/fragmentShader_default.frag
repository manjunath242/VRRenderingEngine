#version 330 core
struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};

struct DirLight {
	vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

};

struct PointLight {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float constant;
    float linear;
    float quadratic;
};

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
	vec4 ViewSpace;
} fs_in;

out vec4 color;

uniform vec3 viewPos;
uniform Material material;
uniform DirLight dirLight;
uniform int NO_OF_POINT_LIGHTS;
uniform PointLight pointLights[140];

const vec3 fogColor = vec3(0.5, 0.5, 0.5);
uniform float FogDensity;

// Function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	// Properties
    vec3 norm = normalize(fs_in.Normal);
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);

	// Phase 1: Directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir);

	// Phase 2: Point lights
    for(int i = 0; i < NO_OF_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, fs_in.FragPos, viewDir);
	
	// To enable Gamma correction uncomment the below line
	//result = pow(result, vec3(1.0/2.2));

	vec3 lightColor = result;

	//distance
	float dist = 0;
	float fogFactor = 0;
	//dist = abs(fs_in.ViewSpace.z);

	dist = length(fs_in.ViewSpace);

	//fogFactor = (80 - dist)/(80 - 20);
	//fogFactor = clamp( fogFactor, 0.0, 1.0 );

	fogFactor = 1.0 /exp(dist * FogDensity);
    fogFactor = clamp( fogFactor, 0.0, 1.0 );

	vec3 finalColor = mix(fogColor, lightColor, fogFactor);
	color = vec4(finalColor, 1);
}

// Calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // Combine results
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, fs_in.TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, fs_in.TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, fs_in.TexCoords));
    return (ambient + diffuse + specular);
}

// Calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
	// For blinn-phong lighting (we calculate an additional halwayDirection and spec calcualtion is changed based on that)
	vec3 halfwayDir = normalize(lightDir + viewDir);
	// For phong lighting model spec is calculated as :  pow(max(dot(viewDir, reflectDir), 0.0), material.shininess)
	// For blinn-phong lighting model, spec is calculated as below (Also the shininess changed to 32 from 16 in mesh.h class)
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // Combine results
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, fs_in.TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, fs_in.TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, fs_in.TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}