//因为着色器源代码这里没有debug，所以非常容易出错
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform sampler2D texture_normal;
uniform vec3 viewPos;
uniform vec3 lightPos1;
uniform vec3 lightPos2;
uniform float shininess;
uniform float specularStrength;
uniform float diffStrength;
uniform float ambientStrength;
void main()
{    
    //环境光照
    vec3 ambient = ambientStrength * texture(texture_diffuse,TexCoords).rgb;

    //漫反射光照
    //vec3 norm = texture(texture_normal,TexCoords).rgb;
    
    //float diffStrength =0.8;

    vec3 norm = normalize(Normal); 
    vec3 lightDir1 = normalize(lightPos1 - FragPos);
    float diff1 = max(dot(norm,lightDir1),0.0) * diffStrength; 

    vec3 lightDir2 = normalize(lightPos2 - FragPos);
    float diff2 = max(dot(norm,lightDir2),0.0) * diffStrength; 

    //高光
     //float specularStrength = 1.0;
     vec3 viewDir = normalize(viewPos - FragPos);
     vec3 halfView1 = normalize(viewDir + lightDir1);
     vec3 reflectDir1 = reflect(-lightDir1,norm);
     float spec1 = pow(max(dot(halfView1,reflectDir1),0.0),shininess);
     vec3 specular1 = texture(texture_specular,TexCoords).rgb * spec1 * specularStrength;

    
     vec3 halfView2 = normalize(viewDir + lightDir2);
     vec3 reflectDir2 = reflect(-lightDir2,norm);
     float spec2 = pow(max(dot(halfView2,reflectDir2),0.0),shininess);
     vec3 specular2 = texture(texture_specular,TexCoords).rgb * spec2 *specularStrength;

    vec3 diffuse = ( diff1 + diff2 * 1.5 + ambient + specular1 + specular2) * texture(texture_diffuse,TexCoords).rgb;

    FragColor = vec4(diffuse, 1.0);
    
}