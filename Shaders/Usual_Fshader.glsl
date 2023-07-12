//因为着色器源代码这里没有debug，所以非常容易出错
#version 330 core

out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

struct Material {
    sampler2D diffuse;

    sampler2D specular; //here
    sampler2D emission; 
    float shininess;
}; 

uniform Material material;
uniform float sin;
struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

void main()
{
    //环境光照
    vec3 ambient = light.ambient * texture(material.diffuse,TexCoords).rgb * lightColor ;
        
    //漫反射光照
    vec3 norm = normalize(Normal); 
    vec3 lightDir = normalize(lightPos-FragPos); //blue 也就是z方向明显错误，原因，没set lightPos，不小心设成了固定值
    float diff = max(dot(norm,lightDir),0.0);  //向量点乘为负数，是为什么？ 答： shader .setVec3(lightPos)必须放在shader.use 之后
    vec3 diffuse = light.diffuse * (diff * texture(material.diffuse,TexCoords).rgb) * lightColor;
    
    //高光 blinn-phong  半程向量
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfView = normalize(viewDir + lightDir);
    vec3 reflectDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(halfView,reflectDir),0.0),material.shininess);
    vec3 specular = light.specular * (texture(material.specular,TexCoords).rgb * spec) * lightColor;

    //发光
    vec3 emission = texture(material.emission,TexCoords).rgb * lightColor * sin;

    //！！问题：
    vec3 result = (ambient + diffuse + specular + emission) * objectColor;
    // vec3 result = (emission) * objectColor ;

    FragColor = vec4(result, 1.0);
}