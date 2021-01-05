#version 420 core
out vec4 FragColor;

in StandardVertexShaderOut
{
	vec3 FragPos; 
	vec2 TexCoords;
} IN;

struct Material {
    sampler2D diffuse;
    vec3 diffuseColor;
};

struct DirLight {
    vec3 ambient;
    vec3 diffuse;
};  

struct PointLight {    
    vec3 position;
    vec3 ambient;
    vec3 diffuse;    
   
    float constant;
    float linear;
    float quadratic;  
};  

#define NR_POINT_LIGHTS 4 

layout (std140, binding = 1) uniform Lights
{
    DirLight dirLight;
    PointLight pointLights[NR_POINT_LIGHTS];
};

uniform Material material;
uniform float alpha;

// function prototypes
vec3 CalcDirLight(DirLight light);  
vec3 CalcPointLight(PointLight light, vec3 fragPos);  

void main()
{
    // phase 1: directional lighting
    vec3 result = CalcDirLight(dirLight);
    // phase 2: point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], IN.FragPos);    

    float imageAlpha = texture(material.diffuse, IN.TexCoords).a;
    result = clamp(result, vec3(0.0f), texture(material.diffuse, IN.TexCoords).rgb);
    FragColor = vec4(result, imageAlpha * alpha);
   // FragColor = vec4(vec3(1.0f), imageAlpha * alpha);
}  

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light)
{
    vec3 matDiffuse = vec3(texture(material.diffuse, IN.TexCoords)) * material.diffuseColor;
    // 결과들을 결합
    vec3 ambient  = light.ambient * matDiffuse;
    vec3 diffuse  = light.diffuse * matDiffuse;
    return (ambient + diffuse);
} 

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 fragPos)
{
    // 거리에 따른 감쇠
    float distance    = length(light.position - fragPos) / 100;
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    

    vec3 matDiffuse = vec3(texture(material.diffuse, IN.TexCoords)) * material.diffuseColor;

    // 결과들을 결합
    vec3 ambient  = light.ambient * matDiffuse;
    vec3 diffuse  = light.diffuse * matDiffuse;

    ambient  *= attenuation;
    diffuse  *= attenuation;

    return (ambient + diffuse);
} 
