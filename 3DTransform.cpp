//�Ƿ���Ҫ��vs�м���ͷ�ļ�����Դ�ļ�

#include <iostream>//ʹ��C++�е�std�����ø�ͷ�ļ�

#include <learnopengl/shader_s.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION  //�������
#include <stb_image.h>
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
	glEnable(GL_DEPTH_TEST); //д�ϻ᲻��ʾ why����������Ϊ��û�漰�����Ϣ �ش𣺲��ǣ���Ϊû��glClear��*|GL_DEPTH_BUFFER_BIT��
	//�������Ƭ����ɫ��Դ����洢���������ļ���
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
	//float vertices[] = {
	//	// positions          // colors           // texture coords
	//	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
	//	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
	//	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
	//	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	//};
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	//֮������������סbuffer��Ҫ�ı䣺������EBO
	/*unsigned int indices[] = {
		0,1,3,
		1,2,3
	};*/

	//����VAO�����Ա��ⲻͣ����ָ�룩
	//����EBO�����Ը���ָ���Ķ��������������ͼ�Σ�
	unsigned int VBO;
	unsigned int VAO;
	//unsigned int EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);//���������ôҲ�����أ�����

	// ���뻺����
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	////put array into element buffer
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//���Ͷ������� //���Ӷ���rgb�󲽳���3����Ϊ6  //����texture�󲽳���Ϊ8
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
		//vetex color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3* sizeof(float)));// ���һ������Ϊƫ����
	glEnableVertexAttribArray(1);
		//texture
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));// ���һ������Ϊƫ����
	//glEnableVertexAttribArray(2);

	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);//ȥ��mipmap����һ��������ǰһ���Ķ���֮һ��
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
			//����FileSystem.getPath(),ֱ�Ӱ�·������������
			//Ҫ�þ���·��������б��
	unsigned char *data = stbi_load("D:/OpenGL/Projects/Test1/test1/resources/textures/container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
		
		//2 png ��ʽ������ȷ
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //ȥ��mipmap����һ��������ǰһ���Ķ���֮һ������ͬ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);  //�ߵ���before loading
	data = stbi_load("D:/OpenGL/Projects/Test1/test1/resources/textures/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{	
		//��������������ɫ��Ϣ��Ϊrgb�����߸�����Ϊrgba����Ϊpng�б�����alpha
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	
	/*glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);*/
	
	////�ӿ�
	//glViewport(0, 0, 800, 600);
	//void framebuffer_size_callback(GLFWwindow * window, int width, int height);

	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	ourShader.use();
	//glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);	//why��
	ourShader.setInt("texture1", 0);			//why��// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	ourShader.setInt("texture2", 1);

	glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	
	//ѭ���ȴ�
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		//rendering commands here

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// also clear the depth buffer now! butwhy��


		//�����
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		//////��ת,����use��������� ��Ȼ����
		//glm::mat4 trans = glm::mat4(1.0f);
		////trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0));
		//trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

			//3d
		/*glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));*/
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		
		ourShader.use();
		
		//unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");//bug:����transformƴ����
		//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		 // pass them to the shaders (3 different ways) --same Result.  Not Found:  ourShader::setMat4
		//currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		//Ŀǰ����ÿ֡����ͶӰ���󣬵�����ͶӰ������ٸı䣬�����������ѭ��֮��ֻ����һ�Ρ� 
		
		/*unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));*/

		unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

		unsigned int projectionLoc = glGetUniformLocation(ourShader.ID, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

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

		//10�ε�ѭ����Ⱦ
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.f * i;
			model = glm::rotate( model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(1.0f, 1.0f, 1.0f));//���һ����������ת��
			//model = model* glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.0f, 0.0f, 1.0f));

			unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);//���� 1��ָ��Ҫ��Ⱦ��ͼԪ���� 
			   // 2��ָ�����������е���ʼ����     3��ָ��Ҫ��Ⱦ�Ķ�����
		//
		}

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//�߿�ģʽ��glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)�ָ�

		
		
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//���� 1��ָ��Ҫ��Ⱦ��ͼԪ���� 
		//	   // 2��ָ��Ҫ���ƵĶ���/Ԫ������     3��ָ��������ֵ������   
		//	   // 4��ָ���������е�ƫ��������ָ������λ�õ�ָ��
		//
		////��ȡ���� VAO ֮ǰ��Ҫȡ����Ԫ�����黺����
		//glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteProgram(shaderProgram);
	//glDeleteBuffers(1, &EBO);
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
