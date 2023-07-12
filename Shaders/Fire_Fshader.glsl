//因为着色器源代码这里没有debug，所以非常容易出错
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

//uniform vec3 objectColor;
uniform sampler2D emission; 

//uniform sampler2D texture_diffuse1;

void main()
{    


    vec3 emission = texture(emission,TexCoords).rgb;

    vec3 result = emission ; //texture_diffuse1
    FragColor = vec4(result, 1.0);
}