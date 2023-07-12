// 是否需要在vs中加载头文件及资源文件

#include <iostream> //使用C++中的std需引用该头文件

#include <learnopengl/shader_s.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION // 加上这句
#include <stb_image.h>
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
// 鼠标灵敏度
const float sensitivity = 0.1f;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
// 全局变量
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

	// 如果是apple系统需加上下面这句：
	// #ifdef _APPLE_
	//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	// #endif

	// 窗口
	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// 回调函数，通过注册registering来告诉 GLFW 我们想要在每次调整窗口大小时调用此函数
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // 这句是什么呢？
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	// stbi_set_flip_vertically_on_load(true);

	glEnable(GL_DEPTH_TEST); // 写上会不显示 why？可能是因为还没涉及深度信息 回答：不是，因为没有glClear（*|GL_DEPTH_BUFFER_BIT）
	// 将顶点和片段着色器源代码存储在这两个文件中
	Shader ourShader("./Shaders/Texture_Vertex_shader.glsl", "./Shaders/Texture_Fragment_shader.glsl");

	Model ourModel("./obj/nanosuit/nanosuit.obj");

	// 循环等待
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		// rendering commands here

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now! butwhy？

		//////旋转,放在use后面可以吗？ 显然可以
		// glm::mat4 trans = glm::mat4(1.0f);
		////trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0));
		// trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

		// 3d
		/*glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));*/

		// 改动
		glm::mat4 view = camera.GetViewMatrix();

		// 摄像机旋转
		// const float radius = 10.f;
		// float camX = sin(glfwGetTime()) * radius;
		// float camZ = 3.0f;//cos(glfwGetTime()) * radius;
		// view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

		// view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);//记得把这里注释掉，否则会覆盖上一条
		glm::mat4 projection = glm::mat4(1.0f);
		// 角度改为相机视角（可动态变化）
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		ourShader.use();

		// unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");//bug:这里transform拼错了
		// glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		// pass them to the shaders (3 different ways) --same Result.  Not Found:  ourShader::setMat4
		// currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		// 目前我们每帧设置投影矩阵，但由于投影矩阵很少改变，所以最好在主循环之外只设置一次。

		/*unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));*/

		unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

		unsigned int projectionLoc = glGetUniformLocation(ourShader.ID, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		//	缺少setMat！ ————> glUniformMatrix4fv————>向shader中传递矩阵

		// glUseProgram(shaderProgram);

		// 这里放置对着色器的具体设置与操作

		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//线框模式，glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)恢复

		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//参数 1、指定要渲染的图元类型
		//	   // 2、指定要绘制的顶点/元素数量     3、指定索引中值的类型
		//	   // 4、指定缓冲区中的偏移量或至指向索引位置的指针
		//
		////在取消绑定 VAO 之前不要取消绑定元素数组缓冲区
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
	// 清理资源
	glfwTerminate();
	return 0;
}
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// 改为使用camera中函数
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

	// 灵敏度

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}