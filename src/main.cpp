#include <glad/glad.h>       // GLAD: load modern OpenGL functions
#include <GLFW/glfw3.h>      // GLFW: window creation + input
#include <iostream>          // for printing errors
#include <thread>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void TestMove(int current, int VBO, int VAO);

// ----------------------------
// 4. Define square vertices, indicies
// ----------------------------
float vertices[] = {
    // positions        // colors
    0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // top right - red
    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right - green
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom left - blue
    -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f // top left - yellow
};


unsigned int indices[] = {
    0, 1, 2, // first triangle
    2, 3, 0 // second triangle
};


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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind VBO, EBO
    glBindVertexArray(0); // unbind VAO// unbind VAO

    // ----------------------------
    // 6. Shader sources
    // ----------------------------
    const char* vertexShaderSource = R"(
    #version 460 core
    layout (location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
    uniform float xOffset;
out vec3 ourColor;
void main() {
    gl_Position = vec4(aPos.x + xOffset, aPos.y, aPos.z, 1.0);
    ourColor = aColor;
}
    )";

    const char* fragmentShaderSource = R"(
    #version 460 core
in vec3 ourColor;
    out vec4 FragColor;
    void main() {
            FragColor = vec4(ourColor, 1.0); // orange
    }
    )";

    // ----------------------------
    // 7. Compile shaders
    // ----------------------------
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // ----------------------------
    // 8. Shader program
    // ----------------------------
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Cleanup shaders; no longer needed once linked
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // ----------------------------
    // 9. Render loop
    // ----------------------------

    bool coolColor = false;

    int test = 0;
    bool forward = true;

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(10.f);
    float xOffset = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT); // clear screen

        glUseProgram(shaderProgram); // use our shader

        if (forward) xOffset += 0.01f;
        else xOffset -= 0.01f;

        if (xOffset >= 0.5f) forward = false;
        else if (xOffset <= -0.5f) forward = true;

        //TestMove(test, VBO, VAO);

        int offsetLoc = glGetUniformLocation(shaderProgram, "xOffset");
        glUniform1f(offsetLoc, xOffset);


        glBindVertexArray(VAO); // bind triangle
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // draw triangle

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
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // ----------------------------
    // 10. Cleanup
    // ----------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void TestMove(int current, int VBO, int VAO)
{
    std::cout << current << std::endl;

    int index = 0;
    for (float vertex : vertices)
    {
        vertices[index] = vertex += current;
        index++;
    }
    //vertices[1] = current;
    //vertices[2] = -current;

    glBindVertexArray(VAO); // bind VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW); // copy data

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
