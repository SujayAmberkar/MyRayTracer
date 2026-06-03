#include<iostream>
#include <GLFW/glfw3.h>
#include "vector"
#include <random>
#include <chrono>

#include "color.h"
#include "vec3.h"

int main(void)
{
    GLFWwindow* window;
    int renderTime = 0;
    int image_width = 800;
    int image_height = 800;
    std::vector<float> pixelValues;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(image_width, image_height, "My RayTracer", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    std::mt19937 rng(42);
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);


    // create a color array
    for (int j = 0; j < image_height; j++) {
        for (int i = 0; i < image_width; i++) {
            auto pixel_color = color(double(i) / (image_width - 1), double(j) / (image_height - 1), 0);

            pixelValues.push_back(pixel_color.x()); // R
            pixelValues.push_back(pixel_color.y()); // G
            pixelValues.push_back(pixel_color.z()); // B
            pixelValues.push_back(1.0f);            //A
            
        }
    }

    

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        auto start = std::chrono::high_resolution_clock::now();
        glDrawPixels(image_width, image_height, GL_RGBA, GL_FLOAT, pixelValues.data());
        auto end = std::chrono::high_resolution_clock::now();
        renderTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    std::cout << renderTime <<"ms";

    glfwTerminate();
    return 0;
}