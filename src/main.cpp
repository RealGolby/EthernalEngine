#include <algorithm>
#include <glad/glad.h>       // GLAD: load modern OpenGL functions
#include <GLFW/glfw3.h>      // GLFW: window creation + input
#include <iostream>          // for printing errors
#include <thread>
#include <vector>
#include <cmath>

#include "Rendering/Shader.h"
#include "Rendering/Images/Texture.h"
#include "stb/stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void TestMove(int current, int VBO, int VAO);

// ----------------------------
// 4. Define square vertices, indicies
// ----------------------------
/*
float vertices[] = {
    // positions          // colors           // texture coords
    0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
    -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f // top left
};
*/

float vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
};

unsigned int indices[] = {
    0, 1, 2, // first triangle
    2, 3, 0 // second triangle
};

float lastX = 400, lastY = 300;
float pitch = 0, yaw = -90;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f);

bool rightClickPressed = false;

bool firstMouse = true;

int main()
{
    // ----------------------------
    // 1. Initialize GLFW
    // ----------------------------
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return -1;
    }

    // Request OpenGL 4.6 Core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // ----------------------------
    // 2. Create a GLFW window
    // ----------------------------
    GLFWwindow* window = glfwCreateWindow(800, 600, "Ethernal Engine", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);


    // ----------------------------
    // 3. Initialize GLAD
    // ----------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD!" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    // Set default background color
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);


    // ----------------------------
    // 5. Create VAO and VBO and EBO
    // ----------------------------
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO); // create VAO
    glGenBuffers(1, &VBO); // create VBO
    glGenBuffers(1, &EBO); // create EBO

    glBindVertexArray(VAO); // bind VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW); // copy data

    // Tell OpenGL how to interpret vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    //glEnableVertexAttribArray(2);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind VBO, EBO
    glBindVertexArray(0); // unbind VAO// unbind VAO

    stbi_set_flip_vertically_on_load(true);

    Shader shaderProgram(R"(C:\Users\golby\CLionProjects\EthernalEngine\src\Rendering\Shaders\main.vsh)", R"(C:\Users\golby\CLionProjects\EthernalEngine\src\Rendering\Shaders\main.fsh)");
    Texture texture(R"(C:\Users\golby\CLionProjects\EthernalEngine\src\Rendering\Images\vasa.png)", 0);
    Texture texture2(R"(C:\Users\golby\CLionProjects\EthernalEngine\src\Rendering\Images\vasaheart.png)", 1);


    bool coolColor = false;

    int test = 0;
    bool forward = true;

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(10.f);
    float xOffset = 0.0f;

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.f, 0.f, 0.f));

    //glm::mat4 view = glm::mat4(1.0f);
    //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    //trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    //trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    float fpsTimer = 0.0f;
    int frames = 0;

    glEnable(GL_DEPTH_TEST);

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
    };

    std::pmr::vector<glm::mat4> matricies;
    for (int i = 0; i < 10; i++)
    {
        matricies.push_back(glm::mat4(1.0f));
    }
    float angle = 0.0f;
    glm::mat4 projection;


    while (!glfwWindowShouldClose(window))
    {
        int fbWidth, fbHeight;
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        projection = glm::perspective(glm::radians(45.0f), float(fbWidth) / float(fbHeight), 0.1f, 100.0f);
        //projection = glm::ortho(0.f,800.f,0.f,600.f,0.1f,100.f);


        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        frames++;
        fpsTimer += deltaTime;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear screen

        if (fpsTimer >= 1.0f)
        {
            std::cout << "FPS: " << frames << std::endl;
            frames = 0;
            fpsTimer = 0.0f;
        }

        shaderProgram.Use(); // use our shader

        if (forward) xOffset += 0.01f;
        else xOffset -= 0.01f;

        if (xOffset >= 0.5f) forward = false;
        else if (xOffset <= -0.5f) forward = true;

        //TestMove(test, VBO, VAO);


        shaderProgram.SetFloat("xOffset", xOffset);
        shaderProgram.SetInt("texture1", 0);
        shaderProgram.SetInt("texture2", 1);

        //glm::mat4 trans = glm::mat4(1.0f);
        //trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        //std::cout << (float)glfwGetTime() << std::endl;
        //trans = glm::rotate(trans, (float) glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

        //unsigned int transformLoc = glGetUniformLocation(shaderProgram.ShaderProgramID, "transform");
        //glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model));

        const float cameraSpeed = 1.05f * deltaTime; // adjust accordingly


        int multiplier = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? 4 : 1;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += cameraSpeed * cameraFront * glm::vec3(multiplier);;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= cameraSpeed * cameraFront * glm::vec3(multiplier);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * glm::vec3(multiplier);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * glm::vec3(multiplier);
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            cameraPos += cameraSpeed * cameraUp * glm::vec3(multiplier);
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            cameraPos -= cameraSpeed * cameraUp * glm::vec3(multiplier);


        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        {
            if (!rightClickPressed) // just pressed this frame
                firstMouse = true;
            rightClickPressed = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else
        {
            rightClickPressed = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }


        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));


        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::mat4 view;
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);


        angle += glm::radians(50.0f) * deltaTime;

        for (int i = 0; i < 10; i++)
        {
            glm::mat4 m = glm::translate(glm::mat4(1.0f), cubePositions[i]);
            //m = glm::rotate(m, angle, glm::vec3(0.5f, 1.0f, 0.0f));

            int modelLoc = glGetUniformLocation(shaderProgram.ShaderProgramID, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(m));
            int viewLoc = glGetUniformLocation(shaderProgram.ShaderProgramID, "view");
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            int projectionLoc = glGetUniformLocation(shaderProgram.ShaderProgramID, "projection");
            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

            glBindVertexArray(VAO); // bind triangle
            //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // draw triangle
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }


        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }


        if (coolColor)
            glClearColor(0, 0, 0, 0);
        else
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        //coolColor = !coolColor;

        glfwSwapBuffers(window); // show rendered frame
        glfwPollEvents(); // handle input/events

        if (forward)
        {
            test++;
        }
        else
        {
            test--;
        }

        if (test >= 10)
        {
            forward = !forward;
        }
        else if (test <= 0)
        {
            forward = !forward;
            //   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        //std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // ----------------------------
    // 10. Cleanup
    // ----------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);


    glfwDestroyWindow(window);
    glfwTerminate();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}



void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (!rightClickPressed) return;

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    pitch = std::clamp(pitch, -89.f, 89.f);

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}
