#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
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
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
};  

#define NR_POINT_LIGHTS 4 

in StandardVertexShaderOut
{
	vec3 FragPos; 
	vec2 TexCoords;
} IN;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform Material material;
uniform float alpha;

// function prototypes
vec3 CalcDirLight(DirLight light, vec3 viewDir);  
vec3 CalcPointLight(PointLight light, vec3 fragPos);  

void main()
{
   // properties
    vec3 viewDir = normalize(viewPos - IN.FragPos);

    // phase 1: directional lighting
    vec3 result = CalcDirLight(dirLight, viewDir);
    // phase 2: point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], IN.FragPos);    

    float imageAlpha = texture(material.diffuse, IN.TexCoords).a;
    FragColor = vec4(result, imageAlpha * alpha);
}  

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 viewDir)
{
    vec3 normal = vec3(0.0f, 0.0f, -1.0f);
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 결과들을 결합
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, IN.TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, IN.TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, IN.TexCoords));
    return (ambient + diffuse + specular);
} 

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 fragPos)
{
    // 거리에 따른 감쇠
    float distance    = length(light.position - fragPos) / 100;
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    

    // 결과들을 결합
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, IN.TexCoords));
    vec3 diffuse  = light.diffuse  * vec3(texture(material.diffuse, IN.TexCoords));

    ambient  *= attenuation;
    diffuse  *= attenuation;

    return (ambient + diffuse);
} 
