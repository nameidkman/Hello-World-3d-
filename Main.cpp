#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    Shader ourShader("3.3.shader.vs", "3.3.shader.fs");


    glEnable(GL_DEPTH_TEST);


    // set up vertex data (and buffer(s)) and configure vertex attributes
    float vertices[] = {
        // positions            // colors             // texture coordinates
        -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,    0.0f, 0.0f,  
         0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,    1.0f, 0.0f,  
         0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,    1.0f, 1.0f,  
         0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,    1.0f, 1.0f,  
        -0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,    0.0f, 1.0f,  
        -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,    0.0f, 0.0f,  

        -0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,    0.0f, 0.0f,  
         0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,    1.0f, 0.0f,  
         0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,    1.0f, 1.0f,  
         0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,    1.0f, 1.0f,  
        -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,    0.0f, 1.0f,  
        -0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,    0.0f, 0.0f,  

        -0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,    1.0f, 0.0f,  
        -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,    1.0f, 1.0f,  
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,    0.0f, 1.0f,  
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,    0.0f, 1.0f,  
        -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,    0.0f, 0.0f,  
        -0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,    1.0f, 0.0f,  

         0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,    1.0f, 0.0f,  
         0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,    1.0f, 1.0f,  
         0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,    0.0f, 1.0f,  
         0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,    0.0f, 1.0f,  
         0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,    0.0f, 0.0f,  
         0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,    1.0f, 0.0f,  

        -0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 1.0f,    0.0f, 1.0f,  
         0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 1.0f,    1.0f, 1.0f,  
         0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 1.0f,    1.0f, 0.0f,  
         0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 1.0f,    1.0f, 0.0f,  
        -0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 1.0f,    0.0f, 0.0f,  
        -0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 1.0f,    0.0f, 1.0f,  

        -0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 1.0f,    0.0f, 1.0f,  
         0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 1.0f,    1.0f, 1.0f,  
         0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 1.0f,    1.0f, 0.0f,  
         0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 1.0f,    1.0f, 0.0f,  
        -0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 1.0f,    0.0f, 0.0f,  
        -0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 1.0f,    0.0f, 1.0f   
    };


    unsigned int index[] = {
        // Front face
        0, 1, 3,  1, 2, 3,
        // Back face
        6, 7, 9,  7, 8, 9,
        // Left face
        12, 13, 15,  13, 14, 15,
        // Right face
        18, 19, 21,  19, 20, 21,
        // Bottom face
        24, 25, 27,  25, 26, 27,
        // Top face
        30, 31, 33,  31, 32, 33
    };
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


    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load("cat.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);  // Free the image data after loading into OpenGL
    }
    else {
        std::cout << "There was a problem with loading the texture" << std::endl;
    }
;

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // Input
        processInput(window);

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Also clear the depth buffer now!

        glBindTexture(GL_TEXTURE_2D, texture);

        // View matrix
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        // Projection matrix
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        // Activate shader and set uniforms
        ourShader.use();
        
       
        // View matrix (single definition)
        glm::mat4 view = glm::mat4(1.0f);

        // Camera movement (rotate over time along a circular path)
        float radius = 10.0f;
        float camX = static_cast<float>(sin(glfwGetTime()) * radius);
        float camZ = static_cast<float>(cos(glfwGetTime()) * radius);
        view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // Camera position that rotates
        ourShader.setMat4("view", view); 
        ourShader.setMat4("projection", projection);



        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]); // Set position of each cube

            // Apply rotation over time (continuous rotation based on time)
            float angle = 20.0f * i + glfwGetTime() * 50.0f;  // Multiply by time to rotate continuously
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f)); // Rotate around an axis

            ourShader.setMat4("model", model);

            // Draw the cube
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

// process all input
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: resize callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
