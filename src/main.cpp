#include <glad/glad.h>       // GLAD: load modern OpenGL functions
#include <GLFW/glfw3.h>      // GLFW: window creation + input
#include <iostream>          // for printing errors

int main() {
    // ----------------------------
    // 1. Initialize GLFW
    // ----------------------------
    if (!glfwInit()) {
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

    if (!window) {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // ----------------------------
    // 3. Initialize GLAD
    // ----------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD!" << std::endl;
        return -1;
    }

    // Set default background color
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // ----------------------------
    // 4. Define triangle vertices
    // ----------------------------
    float vertices[] = {
         0.0f,  0.5f, 0.0f,  // top
        -0.5f, -0.5f, 0.0f,  // bottom-left
         0.5f, -0.5f, 0.0f ,  // bottom-right
    };

    // ----------------------------
    // 5. Create VAO and VBO
    // ----------------------------
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);       // create VAO
    glGenBuffers(1, &VBO);            // create VBO

    glBindVertexArray(VAO);           // bind VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // copy data

    // Tell OpenGL how to interpret vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind VBO
    glBindVertexArray(0);             // unbind VAO// unbind VAO

    // ----------------------------
    // 6. Shader sources
    // ----------------------------
    const char* vertexShaderSource = R"(
    #version 460 core
    layout (location = 0) in vec3 aPos;
    void main() {
        gl_Position = vec4(aPos, 1.0);
    }
    )";

    const char* fragmentShaderSource = R"(
    #version 460 core
    out vec4 FragColor;
    void main() {
        FragColor = vec4(1.0, 0.5, 0.2, 1.0); // orange
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
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);       // clear screen

        glUseProgram(shaderProgram);        // use our shader
        glBindVertexArray(VAO);             // bind triangle
        glDrawArrays(GL_TRIANGLES, 0, 3);   // draw triangle

        glfwSwapBuffers(window);            // show rendered frame
        glfwPollEvents();                   // handle input/events
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
