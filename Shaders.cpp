#include <iostream>//ʹ��C++�е�std�����ø�ͷ�ļ�

#include <learnopengl/shader_s.h>//����Ŀ¼D:\APP\VSIDE\VC\Tools\MSVC\14.32.31326\include��

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//�ⲿ��Ҳ��д������ɫ����
/*
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"//BUG!!����ط�p�ʼû�д�д
"layout (location = 1) in vec3 aColor;\n"

"out vec3 ourColor;\n"
	//����vertexColor�����������
//"out vec4 vertexColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"   ourColor = aColor;\n"
	//"vertexColor = vec4(0.5,0.0,0.0,1.0);\n"	
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
	//Ƭ����ɫ���������Զ�����ɫ���������Ϊ������
//"in vec4 vertexColor;\n"
//"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor =vec4(ourColor,1.0);\n"
"}\n\0";
*/

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//�����appleϵͳ�����������䣺
//#ifdef _APPLE_
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif

	//����
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//�ص�������ͨ��ע��registering������ GLFW ������Ҫ��ÿ�ε������ڴ�Сʱ���ô˺���
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//�����ʲô�أ�

	//GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Shader ourShader("D:/OpenGL/Projects/Test1/test1/Shaders/Texture_Vertex_shader.glsl", "D:/OpenGL/Projects/Test1/test1/Shaders/Texture_Fragment_shader.glsl");
	//�ⲿ�ֶ�д������ɫ����
	/*
	//����������ɫ��
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	//����1
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Ƭ����ɫ��
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);//���������ô�����أ�����
		//����2
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Shader program ��������ɫ����Ƭ����ɫ��������һ��
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	//����3
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	//����Ϳ���ɾ����
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	*/
	float vertices[] = {
		// positions         // colors
	 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
	 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
	};
	unsigned int indices[] = {
		0,1,3,
		1,2,3
	};

	//����VAO�����Ա��ⲻͣ����ָ�룩
	//����EBO�����Ը���ָ���Ķ��������������ͼ�Σ�
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);//���������ôҲ�����أ�����

	// ���뻺����
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//put array into element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//���Ͷ������� //���Ӷ���rgb�󲽳���3����Ϊ6
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
		//vetex color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));// ���һ������Ϊƫ����
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//�ӿ�
	glViewport(0, 0, 800, 600);
	void framebuffer_size_callback(GLFWwindow * window, int width, int height);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//ѭ���ȴ�
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		//rendering commands here

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ourShader.use();
		//glUseProgram(shaderProgram);
		
		//������ö���ɫ���ľ������������
		

		//update the uniform color
		/*
		float timeValue = glfwGetTime();
		float greenValue = sin(timeValue) / 2.0f + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		*/

		glBindVertexArray(VAO);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//�߿�ģʽ��glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)�ָ�

		glDrawArrays(GL_TRIANGLES, 0, 3);//���� 1��ָ��Ҫ��Ⱦ��ͼԪ���� 
			   // 2��ָ�����������е���ʼ����     3��ָ��Ҫ��Ⱦ�Ķ�����
		
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//���� 1��ָ��Ҫ��Ⱦ��ͼԪ���� 
		//	   // 2��ָ��Ҫ���ƵĶ���/Ԫ������     3��ָ��������ֵ������   
		//	   // 4��ָ���������е�ƫ��������ָ������λ�õ�ָ��
		////��ȡ���� VAO ֮ǰ��Ҫȡ����Ԫ�����黺����
		//glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteProgram(shaderProgram);

	//������Դ
	glfwTerminate();
	return 0;
}
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
