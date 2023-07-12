// �Ƿ���Ҫ��vs�м���ͷ�ļ�����Դ�ļ�

#include <iostream> //ʹ��C++�е�std�����ø�ͷ�ļ�

#include <learnopengl/shader_s.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION // �������
#include <stb_image.h>
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
// ���������
const float sensitivity = 0.1f;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
// ȫ�ֱ���
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
// float yaw = -90.0f;
// float pitch = -0.0f;
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
// float fov = 45.0f;

float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// �����appleϵͳ�����������䣺
	// #ifdef _APPLE_
	//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	// #endif

	// ����
	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// �ص�������ͨ��ע��registering������ GLFW ������Ҫ��ÿ�ε������ڴ�Сʱ���ô˺���
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // �����ʲô�أ�
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	// stbi_set_flip_vertically_on_load(true);

	glEnable(GL_DEPTH_TEST); // д�ϻ᲻��ʾ why����������Ϊ��û�漰�����Ϣ �ش𣺲��ǣ���Ϊû��glClear��*|GL_DEPTH_BUFFER_BIT��
	// �������Ƭ����ɫ��Դ����洢���������ļ���
	Shader ourShader("./Shaders/Texture_Vertex_shader.glsl", "./Shaders/Texture_Fragment_shader.glsl");

	Model ourModel("./obj/nanosuit/nanosuit.obj");

	// ѭ���ȴ�
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		// rendering commands here

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now! butwhy��

		//////��ת,����use��������� ��Ȼ����
		// glm::mat4 trans = glm::mat4(1.0f);
		////trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0));
		// trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

		// 3d
		/*glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));*/

		// �Ķ�
		glm::mat4 view = camera.GetViewMatrix();

		// �������ת
		// const float radius = 10.f;
		// float camX = sin(glfwGetTime()) * radius;
		// float camZ = 3.0f;//cos(glfwGetTime()) * radius;
		// view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

		// view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);//�ǵð�����ע�͵�������Ḳ����һ��
		glm::mat4 projection = glm::mat4(1.0f);
		// �Ƕȸ�Ϊ����ӽǣ��ɶ�̬�仯��
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		ourShader.use();

		// unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");//bug:����transformƴ����
		// glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		// pass them to the shaders (3 different ways) --same Result.  Not Found:  ourShader::setMat4
		// currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		// Ŀǰ����ÿ֡����ͶӰ���󣬵�����ͶӰ������ٸı䣬�����������ѭ��֮��ֻ����һ�Ρ�

		/*unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));*/

		unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

		unsigned int projectionLoc = glGetUniformLocation(ourShader.ID, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		//	ȱ��setMat�� ��������> glUniformMatrix4fv��������>��shader�д��ݾ���

		// glUseProgram(shaderProgram);

		// ������ö���ɫ���ľ������������

		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//�߿�ģʽ��glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)�ָ�

		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//���� 1��ָ��Ҫ��Ⱦ��ͼԪ����
		//	   // 2��ָ��Ҫ���ƵĶ���/Ԫ������     3��ָ��������ֵ������
		//	   // 4��ָ���������е�ƫ��������ָ������λ�õ�ָ��
		//
		////��ȡ���� VAO ֮ǰ��Ҫȡ����Ԫ�����黺����
		// glBindVertexArray(0);
		ourModel.Draw(ourShader);

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glDeleteProgram(shaderProgram);
	// glDeleteBuffers(1, &EBO);
	// ������Դ
	glfwTerminate();
	return 0;
}
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// ��Ϊʹ��camera�к���
	// const float cameraSpeed = 2.5f*deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	// ������

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}