//��Ϊ��ɫ��Դ��������û��debug�����Էǳ����׳���
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
    //��������
    vec3 ambient = light.ambient * texture(material.diffuse,TexCoords).rgb * lightColor ;
        
    //���������
    vec3 norm = normalize(Normal); 
    vec3 lightDir = normalize(lightPos-FragPos); //blue Ҳ����z�������Դ���ԭ��ûset lightPos����С������˹̶�ֵ
    float diff = max(dot(norm,lightDir),0.0);  //�������Ϊ��������Ϊʲô�� �� shader .setVec3(lightPos)�������shader.use ֮��
    vec3 diffuse = light.diffuse * (diff * texture(material.diffuse,TexCoords).rgb) * lightColor;
    
    //�߹� blinn-phong  �������
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfView = normalize(viewDir + lightDir);
    vec3 reflectDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(halfView,reflectDir),0.0),material.shininess);
    vec3 specular = light.specular * (texture(material.specular,TexCoords).rgb * spec) * lightColor;

    //����
    vec3 emission = texture(material.emission,TexCoords).rgb * lightColor * sin;

    //�������⣺
    vec3 result = (ambient + diffuse + specular + emission) * objectColor;
    // vec3 result = (emission) * objectColor ;

    FragColor = vec4(result, 1.0);
}