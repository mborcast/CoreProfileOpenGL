#version 330 core
struct Material 
{
    vec3 diffuse;
    vec3 specular;
    float shininess;
};  

struct Light 
{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

in vec3 FragPos;  
in vec3 Normal;  
  
out vec4 color;
  
uniform vec3 viewPos;
uniform Material material;
uniform Light lightA;
uniform Light lightB;

void main()
{
    //LIGHT A

    // Ambient
    vec3 ambient = lightA.ambient * material.diffuse;
    
    // Diffuse 
    vec3 norm = normalize(Normal);        
    vec3 lightDir = normalize(lightA.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightA.diffuse * diff * material.diffuse;  
    
    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = lightA.specular * spec * material.specular;
    
    // Spotlight (soft edges)
    float theta = dot(lightDir, normalize(-lightA.direction)); 
    float epsilon = (lightA.cutOff - lightA.outerCutOff);
    float intensity = clamp((theta - lightA.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;
    
    // Attenuation
    float distance    = length(lightA.position - FragPos);
    float attenuation = 1.0f / (lightA.constant + lightA.linear * distance + lightA.quadratic * (distance * distance));    
    ambient  *= attenuation; 
    diffuse  *= attenuation;
    specular *= attenuation;   

    vec4 colorA = vec4(ambient + diffuse + specular, 1.0f); 

    //LIGHT B
    ambient = lightB.ambient * material.diffuse;
    
    // Diffuse 
    norm = normalize(Normal);        
    lightDir = normalize(lightB.position - FragPos);
    diff = max(dot(norm, lightDir), 0.0);
    diffuse = lightB.diffuse * diff * material.diffuse;  
    
    // Specular
    viewDir = normalize(viewPos - FragPos);
    reflectDir = reflect(-lightDir, norm);  
    spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    specular = lightB.specular * spec * material.specular;
    
    // Spotlight (soft edges)
    theta = dot(lightDir, normalize(-lightB.direction)); 
    epsilon = (lightB.cutOff - lightB.outerCutOff);
    intensity = clamp((theta - lightB.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;
    
    // Attenuation
    distance    = length(lightB.position - FragPos);
    attenuation = 1.0f / (lightB.constant + lightB.linear * distance + lightB.quadratic * (distance * distance));    
    ambient  *= attenuation; 
    diffuse  *= attenuation;
    specular *= attenuation;   

    vec4 colorB = vec4(ambient + diffuse + specular, 1.0f); 

    color = colorB + colorA; 
} 