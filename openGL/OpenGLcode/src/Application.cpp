#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shade.h"                 
#include "camera.h"
#include "model.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);
unsigned int loadCubemap(vector<std::string> faces);

// 窗口设置
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1000;

// 摄像机
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// 间隔时间
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// 光照
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
    // 初始化设置
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // 创建窗口
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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

    //捕捉鼠标
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // 加载GLAD
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 开启深度测试
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // 绑定着色器
    // ------------------------------------
    Shader lightingShader("5.2.light_casters.vs", "5.2.light_casters.fs");
    Shader WallShader("5.2.light_casters.vs", "5.2.light_casters.fs");
    Shader MaterialShader("material.vs", "material.fs");
    Shader lightCubeShader("5.2.light_cube.vs", "5.2.light_cube.fs");
    Shader reflectcubeshader("6.2.cubemaps.vs", "6.2.cubemaps.fs");
    Shader reflactcubeshader("6.2.cubemaps.vs", "reflaction.fs");
    Shader skyboxShader("6.2.skybox.vs", "6.2.skybox.fs");



    // 设置顶点矩阵
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    float cubeVertices[] = {
        // positions          // normals
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    //天空盒坐标
    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    // 物体位置
    glm::vec3 cubePositions[] = {
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f),
        glm::vec3(2.0f, 2.0f, -12.0f)
    };
    // 多个点光源
    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f,  0.2f,  2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3(0.0f,  0.0f, -3.0f)
    };
    // 设置物体的VAO和VBO
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // 设置点光源的VAO和VBO
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 更新位置信息来更新容器
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 加载纹理
// -----------------------------------------------------------------------------
    unsigned int diffuseMap = loadTexture("source/container2.png");
    unsigned int specularMap = loadTexture("source/container2_specular.png");
    unsigned int diffuseMap2 = loadTexture("source/wall2.bmp");

    // 着色器设置
    // --------------------
    lightingShader.use();
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);

    WallShader.use();
    WallShader.setInt("material.diffuse", 0);
    WallShader.setInt("material.specular", 1);


    // 反光 cube VAO
    unsigned int reflectcubeVAO, reflectcubeVBO;
    glGenVertexArrays(1, &reflectcubeVAO);
    glGenBuffers(1, &reflectcubeVBO);
    glBindVertexArray(reflectcubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, reflectcubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    //天空盒
    // skybox VAO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // load textures
    // -------------
    vector<std::string> faces
    {
        "source/skybox2/right.jpg",
        "source/skybox2/left.jpg",
        "source/skybox2/top.jpg",
        "source/skybox2/bottom.jpg",
        "source/skybox2/front.jpg",
        "source/skybox2/back.jpg",
    };
    unsigned int cubemapTexture = loadCubemap(faces);

    // shader configuration
    // --------------------
    reflectcubeshader.use();
    reflectcubeshader.setInt("skybox", 0);

    reflactcubeshader.use();
    reflactcubeshader.setInt("skybox", 0);

    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);


    // 循环
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // 时间逻辑
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // 输入
        // -----
        processInput(window);

        // render
        // ------
        //背景色
        glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 激活着色器
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);
        //物体材质
        lightingShader.setFloat("material.shininess", 32.0f);

        // 平行光
        lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        lightingShader.setVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
        lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        //点光源 1
        lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
        lightingShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[0].constant", 1.0f);
        lightingShader.setFloat("pointLights[0].linear", 0.09);
        lightingShader.setFloat("pointLights[0].quadratic", 0.032);
        // 点光源 2
        lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
        lightingShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[1].constant", 1.0f);
        lightingShader.setFloat("pointLights[1].linear", 0.09);
        lightingShader.setFloat("pointLights[1].quadratic", 0.032);
        //点光源 3
        lightingShader.setVec3("pointLights[2].position", pointLightPositions[2]);
        lightingShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[2].constant", 1.0f);
        lightingShader.setFloat("pointLights[2].linear", 0.09);
        lightingShader.setFloat("pointLights[2].quadratic", 0.032);
        // 点光源 4
        lightingShader.setVec3("pointLights[3].position", pointLightPositions[3]);
        lightingShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[3].constant", 1.0f);
        lightingShader.setFloat("pointLights[3].linear", 0.09);
        lightingShader.setFloat("pointLights[3].quadratic", 0.032);
        // 汇聚光
        lightingShader.setVec3("spotLight.position", camera.Position);
        lightingShader.setVec3("spotLight.direction", camera.Front);
        lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        lightingShader.setVec3("spotLight.diffuse", 0.0f, 0.0f, 0.0f);
        lightingShader.setVec3("spotLight.specular", 0.0f, 0.0f, 0.0f);
        lightingShader.setFloat("spotLight.constant", 1.0f);
        lightingShader.setFloat("spotLight.linear", 0.09);
        lightingShader.setFloat("spotLight.quadratic", 0.032);
        lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));


        // 视角、投影变换
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // 世界坐标变换
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);

        // 绑定漫反射属性
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        // 绑定镜面反射属性
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        // 贴图物体VAO
        glBindVertexArray(cubeVAO);
        for (unsigned int i = 0; i < 9; i += 3)
        {
            // 计算物体的属性
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 10.0f * i * glfwGetTime();
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            lightingShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // 激活着色器
        WallShader.use();
        WallShader.setVec3("viewPos", camera.Position);
        //物体材质
        WallShader.setFloat("material.shininess", 32.0f);

        // 平行光
        WallShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        WallShader.setVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
        WallShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        WallShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        //点光源 1
        WallShader.setVec3("pointLights[0].position", pointLightPositions[0]);
        WallShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        WallShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        WallShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        WallShader.setFloat("pointLights[0].constant", 1.0f);
        WallShader.setFloat("pointLights[0].linear", 0.09);
        WallShader.setFloat("pointLights[0].quadratic", 0.032);
        // 点光源 2
        WallShader.setVec3("pointLights[1].position", pointLightPositions[1]);
        WallShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        WallShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        WallShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        WallShader.setFloat("pointLights[1].constant", 1.0f);
        WallShader.setFloat("pointLights[1].linear", 0.09);
        WallShader.setFloat("pointLights[1].quadratic", 0.032);
        //点光源 3
        WallShader.setVec3("pointLights[2].position", pointLightPositions[2]);
        WallShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        WallShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        WallShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        WallShader.setFloat("pointLights[2].constant", 1.0f);
        WallShader.setFloat("pointLights[2].linear", 0.09);
        WallShader.setFloat("pointLights[2].quadratic", 0.032);
        // 点光源 4
        WallShader.setVec3("pointLights[3].position", pointLightPositions[3]);
        WallShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        WallShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        WallShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        WallShader.setFloat("pointLights[3].constant", 1.0f);
        WallShader.setFloat("pointLights[3].linear", 0.09);
        WallShader.setFloat("pointLights[3].quadratic", 0.032);
        // 汇聚光
        WallShader.setVec3("spotLight.position", camera.Position);
        WallShader.setVec3("spotLight.direction", camera.Front);
        WallShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        WallShader.setVec3("spotLight.diffuse", 0.0f, 0.0f, 0.0f);
        WallShader.setVec3("spotLight.specular", 0.0f, 0.0f, 0.0f);
        WallShader.setFloat("spotLight.constant", 1.0f);
        WallShader.setFloat("spotLight.linear", 0.09);
        WallShader.setFloat("spotLight.quadratic", 0.032);
        WallShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        WallShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));


        // 视角、投影变换
        WallShader.setMat4("projection", projection);
        WallShader.setMat4("view", view);

        // 世界坐标变换
        WallShader.setMat4("model", model);

        // 绑定漫反射属性
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap2);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);

        //砖块
        glBindVertexArray(cubeVAO);
        for (unsigned int i = 2; i < 9; i += 3)
        {
            // 计算物体的属性
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 10.0f * i * glfwGetTime();
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            WallShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }



        // 反光物体
        reflectcubeshader.use();
        reflectcubeshader.setMat4("model", model);
        reflectcubeshader.setMat4("view", view);
        reflectcubeshader.setMat4("projection", projection);
        reflectcubeshader.setVec3("cameraPos", camera.Position);
        glBindVertexArray(reflectcubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        //折射物体
        reflactcubeshader.use();
        model = glm::translate(model, cubePositions[9]);
        reflactcubeshader.setMat4("model", model);
        reflactcubeshader.setMat4("view", view);
        reflactcubeshader.setMat4("projection", projection);
        reflactcubeshader.setVec3("cameraPos", camera.Position);
        glBindVertexArray(reflectcubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        
        // 物体材质
        MaterialShader.use();
        MaterialShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
        MaterialShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
        MaterialShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f); 
        MaterialShader.setFloat("material.shininess", 32.0f);

        // 平行光
        MaterialShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        MaterialShader.setVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
        MaterialShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        MaterialShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
  
        MaterialShader.setVec3("pointLights[0].position", pointLightPositions[0]);
        MaterialShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        MaterialShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        MaterialShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        MaterialShader.setFloat("pointLights[0].constant", 1.0f);
        MaterialShader.setFloat("pointLights[0].linear", 0.09);
        MaterialShader.setFloat("pointLights[0].quadratic", 0.032);
    
        MaterialShader.setVec3("pointLights[1].position", pointLightPositions[1]);
        MaterialShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        MaterialShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        MaterialShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        MaterialShader.setFloat("pointLights[1].constant", 1.0f);
        MaterialShader.setFloat("pointLights[1].linear", 0.09);
        MaterialShader.setFloat("pointLights[1].quadratic", 0.032);
      
        MaterialShader.setVec3("pointLights[2].position", pointLightPositions[2]);
        MaterialShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        MaterialShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        MaterialShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        MaterialShader.setFloat("pointLights[2].constant", 1.0f);
        MaterialShader.setFloat("pointLights[2].linear", 0.09);
        MaterialShader.setFloat("pointLights[2].quadratic", 0.032);
    
        MaterialShader.setVec3("pointLights[3].position", pointLightPositions[3]);
        MaterialShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        MaterialShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        MaterialShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        MaterialShader.setFloat("pointLights[3].constant", 1.0f);
        MaterialShader.setFloat("pointLights[3].linear", 0.09);
        MaterialShader.setFloat("pointLights[3].quadratic", 0.032);
     
        MaterialShader.setVec3("spotLight.position", camera.Position);
        MaterialShader.setVec3("spotLight.direction", camera.Front);
        MaterialShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        MaterialShader.setVec3("spotLight.diffuse", 0.0f, 0.0f, 0.0f);
        MaterialShader.setVec3("spotLight.specular", 0.0f, 0.0f, 0.0f);
        MaterialShader.setFloat("spotLight.constant", 1.0f);
        MaterialShader.setFloat("spotLight.linear", 0.09);
        MaterialShader.setFloat("spotLight.quadratic", 0.032);
        MaterialShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        MaterialShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
    
        MaterialShader.setMat4("projection", projection);
        MaterialShader.setMat4("view", view);

        // 绑定漫反射属性
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap2);
    
        // 材质物体VAO
        glBindVertexArray(reflectcubeVAO);
        for (unsigned int i = 1; i < 9; i+=3)
        {
            // 计算物体的属性
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 5.0f * i* glfwGetTime();
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            lightingShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

       
        // 计算光源位置
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        // 为四个点光源设置
        glBindVertexArray(lightCubeVAO);
        for (unsigned int i = 0; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); //小方块
            lightCubeShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }



        // 最后绘制天空盒
        glDepthFunc(GL_LEQUAL);  // 改变深度测试函数
        skyboxShader.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // 移除视角矩阵
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // 设置默认深度测试函数



        //清空容器
        // -------------------------------------------------------------------------------
            //清空容器
        //glDeleteVertexArrays(1, &reflectcubeVAO);
        //glDeleteVertexArrays(1, &skyboxVAO);
        //glDeleteBuffers(1, &reflectcubeVBO);
        //glDeleteBuffers(1, &skyboxVAO);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 清空容器
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);


    glfwTerminate();
    return 0;
}


void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{

    glViewport(0, 0, width, height);
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
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

    camera.ProcessMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}


unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrComponents;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}