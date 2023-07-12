#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader_s.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>
#include <learnopengl/mesh.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif
// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

const char *glsl_version = "#version 130";

#define PI 3.1415926f

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;

// camera
glm::vec3 cameraPosition = glm::vec3(-9.63f, 154.82f, 88.81f);
glm::vec3 firePosition = glm::vec3(1.0f, 65.0f, 0.0f);
// glm::vec3 cameraRight = glm::vec3(1.0f, 0.0f, 00.0f);
// glm::vec3 cameraFront=glm::normalize(glm::cross(cameraPosition, cameraRight));
// Camera camera(cameraPosition, cameraFront);
Camera camera(cameraPosition);
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float timer = 0.0f;

float f = 0.002f * PI;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();

// glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
// glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
// #ifdef __APPLE__
//     glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
// #endif
// Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char *glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char *glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "In_vessel_Fish", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    // std::cout<<cameraFront.x<<' ' << cameraFront.y<<' ' << cameraFront.z << endl;
    //  tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(false);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    // glfwSwapInterval(1); // Enable vsync 垂直同步 不开

    // build and compile shaders
    // -------------------------
    Shader ourShader("./Shaders/Texture_Vertex_shader.glsl", "./Shaders/Texture_Fragment_shader.glsl");
    Shader lightShader("./Shaders/Fire_Vshader.glsl", "./Shaders/Fire_Fshader.glsl");
    // Shader fishShader("./Shaders/Fish_Vshader.glsl", "./Shaders/Fish_Fshader.glsl");

    // 1. load models加载模型
    // -----------
    Model fish("./model/goldenfish/Goldfish_v1.obj");
    Model cup("./model/CupandHolder_Texture2K/CupandHolder_Texture2K/Cup_and_Holder_OBJ.obj");
    Model candle("./model/CandelStick/CandleStick.obj");
    Model floor("./model/floor/StoneFloor_SlabTwo_LOD2.obj"); // StoneFloor_SlabTwo_LOD2.obj
    Model fire("./model/fire/fire.obj");
    Model lamp("./model/Lamp/Lamp.obj");

    // draw in wireframe
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    int width, height, nrChannels;
    unsigned int emissionMap;
    glGenTextures(1, &emissionMap);
    glBindTexture(GL_TEXTURE_2D, emissionMap);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // 去掉mipmap（后一个纹理是前一个的二分之一）
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char *emissionData = stbi_load("./model/fire/fire_diff1.jpg", &width, &height, &nrChannels, 0);
    if (emissionData)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, emissionData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(emissionData);

    unsigned int lampEemissionMap;
    glGenTextures(1, &lampEemissionMap);
    glBindTexture(GL_TEXTURE_2D, lampEemissionMap);

    unsigned char *lampEemissionData = stbi_load("./model/Lamp/Lamp_Emission.jpg", &width, &height, &nrChannels, 0);
    if (lampEemissionData)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, lampEemissionData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(lampEemissionData);

    // 2. 模型变换矩阵
    glm::mat4 fishmodel = glm::mat4(1.0f);
    fishmodel = glm::translate(fishmodel, glm::vec3(19.0f, 35.0f, -28.50f)); // translate it down so it's at the center of the scene
    fishmodel = glm::scale(fishmodel, glm::vec3(2.0, 2.0, 2.0));             // it's a bit too big for our scene, so scale it down

    glm::mat4 cupmodel = glm::mat4(1.0f);
    cupmodel = glm::translate(cupmodel, glm::vec3(15.0f, -13.0f, 0.0f)); // glm::vec3(4.0f, 0.0f, -1.0f));
    cupmodel = glm::scale(cupmodel, glm::vec3(1.5f, 1.5f, 1.5f));

    glm::mat4 candlemodel = glm::mat4(1.0f);
    candlemodel = glm::translate(candlemodel, glm::vec3(0.0f, 5.0f, 0.0f)); // glm::vec3(4.0f, 0.0f, -1.0f));
    candlemodel = glm::scale(candlemodel, glm::vec3(1.0f, 1.4f, 1.0f));
    //
    glm::mat4 firemodel = glm::mat4(1.0f);
    firemodel = glm::translate(candlemodel, firePosition);
    firemodel = glm::scale(firemodel, glm::vec3(5.0f, 5.0f, 5.0f));

    /*glm::mat4 fire2model = glm::mat4(1.0f);
    fire2model = glm::translate(candlemodel, firePosition);
    fire2model = glm::scale(fire2model, glm::vec3(5.0f, 5.0f, 5.0f));*/

    glm::mat4 lampmodel = glm::mat4(1.0f);
    lampmodel = glm::translate(candlemodel, firePosition);
    lampmodel = glm::scale(lampmodel, glm::vec3(1.0f, 1.0f, 1.0f));
    lampmodel = glm::rotate(lampmodel, (float)glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    lampmodel = glm::translate(lampmodel, glm::vec3(0.0f, -15.0f, 0.0f));

    glm::mat4 floormodel = glm::mat4(1.0f);
    floormodel = glm::translate(floormodel, glm::vec3(0.0f, -15.0f, 0.0f));
    floormodel = glm::scale(floormodel, glm::vec3(20.0f, 1.0f, 20.0f));

    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);

    glm::mat4 origin = glm::mat4(1.0f);
    glm::mat4 tr = glm::mat4(1.0f);
    glm::mat4 ro = glm::mat4(1.0f);

    lightShader.use();
    lightShader.setInt("emission", 0);

    // Prepare GUI
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // view/projection transformations
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        view = camera.GetViewMatrix();

        // don't forget to enable shader before setting uniforms

        //  一般物体
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        ourShader.setVec3("viewPos", camera.Position);
        ourShader.setVec3("lightPos1", glm::vec3(firemodel * glm::vec4(firePosition, 1.0)));
        ourShader.setVec3("lightPos2", glm::vec3(lampmodel * glm::vec4(firePosition, 1.0)));

        ourShader.setFloat("shininess", 2.0f);
        ourShader.setFloat("specularStrength", 0.5f);
        ourShader.setFloat("diffStrength", 0.8f);
        ourShader.setFloat("ambientStrength", 0.1f);

        // render the loaded model
        timer += 0.0002f;
        float radius = 0.000002f; // float time = (float)glfwGetTime();   //std::cout << "上一帧是：" << timer << "秒" << endl;
        float s = 0.005f;
        tr = glm::translate(origin, glm::vec3((float)cos(glm::radians(timer * s)) * radius, 0.0f, (float)sin(glm::radians(timer * s)) * radius)); //(float)sin(glm::radians(timer / 360.00f)) * radius)
        ro = glm::rotate(origin, -glm::radians(timer * s), glm::vec3(0.0f, 1.0f, 0.0f));
        fishmodel = tr * ro * fishmodel; // std::cout << camera.Position.x<< ' '<<camera.Position.y<<' '<<camera.Position.z;

        //      3 （结束）. 将模型矩阵导入着色器
        unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(cupmodel));
        // ourShader.setMat4("model", cupmodel);
        cup.Draw(ourShader);

        ourShader.setFloat("diffStrength", 1.0f);
        ourShader.setFloat("ambientStrength", 0.3f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(candlemodel));
        candle.Draw(ourShader);

        ourShader.setFloat("shininess", 1.0f);
        ourShader.setFloat("specularStrength", 0.0f);
        ourShader.setFloat("diffStrength", 0.2f);
        ourShader.setFloat("ambientStrength", 0.1f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(floormodel));
        floor.Draw(ourShader);

        ourShader.setFloat("shininess", 32.0f);
        ourShader.setFloat("specularStrength", 100.0f);
        ourShader.setFloat("diffStrength", 1.00f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(fishmodel));
        fish.Draw(ourShader);

        //  发光体
        lightShader.use();
        lightShader.setMat4("projection", projection);
        lightShader.setMat4("view", view);

        // Fire
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, emissionMap);
        glm::mat4 lightModel = glm::mat4(1.0f);
        // lightModel = glm::translate(lightModel, firemodel);
        // lightModel = glm::scale(lightModel, glm::vec3(0.2f));
        firemodel = glm::translate(firemodel, glm::vec3(glm::cos(timer * s * 500.0f) / 5000.0f, glm::cos(timer * s * 10000.0f) / 5000.0f, 0.0f));

        unsigned int lightModelLoc = glGetUniformLocation(lightShader.ID, "model");
        glUniformMatrix4fv(lightModelLoc, 1, GL_FALSE, glm::value_ptr(firemodel));
        fire.Draw(lightShader);

        // Lamp
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, lampEemissionMap);
        lampmodel = glm::translate(lampmodel, glm::vec3(glm::sin(PI / 2.0f + (timer * s * 300.0f)) / 25.0f, 0.0f, 0.0f));

        unsigned int lampModelLoc = glGetUniformLocation(lightShader.ID, "model");
        glUniformMatrix4fv(lampModelLoc, 1, GL_FALSE, glm::value_ptr(lampmodel));
        lamp.Draw(lightShader);

        /*fire2model = glm::translate(fire2model, glm::vec3(glm::sin(PI/2.0f+(timer * s * 300.0f)) / 50.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(lightModelLoc, 1, GL_FALSE, glm::value_ptr(fire2model));
        fire2.Draw(lightShader);*/

        // glDrawArrays(GL_TRIANGLES, 0, 36);//参数 1、指定要渲染的图元类型
        //    // 2、指定启用数组中的起始索引 3、指定要渲染的顶点数
        //// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// void Sleep(float interval) {
//     float time = glfwGetTime();
//     while (glfwGetTime() - time < (interval/1000))
//     {
//     };
// }
//  process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
//  ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float v = deltaTime * 10;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, v);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, v);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, v);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, v);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
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
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
