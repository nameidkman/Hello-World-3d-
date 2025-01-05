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
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* winodw, double xOffset, double yOffset);


glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float deltaTime = 0.0f;
float lastFrame = 0.0f;
glm::mat4 projection = glm::mat4(1.0f);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
glm::mat4 view = glm::mat4(1.0f);

bool firstMouse = true;
float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
float fov   =  45.0f;


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

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);




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
        


        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame; 
        lastFrame = currentFrame; 


        // Input
        processInput(window);

        // Render
        /*glClearColor(0.2f, 0.3f, 0.3f, 1.0f)*/;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Also clear the depth buffer now!

        glBindTexture(GL_TEXTURE_2D, texture);
        
        
        ourShader.use();
        
        projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH/ (float)SCR_HEIGHT, 0.1f, 100.0f);
        

        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp); 
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
    const float cameraSpeed = 2.5f * deltaTime; 

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront; 
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront; 
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cameraPos  += cameraSpeed * cameraUp; 
    if (glfwGetKey(window , GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraUp; 
}
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
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

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}

// glfw: resize callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
