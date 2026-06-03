#include<iostream>
#include <GLFW/glfw3.h>
#include "vector"
#include <random>
#include <chrono>

int main(void)
{
    GLFWwindow* window;
    int renderTime = 0;
    int width = 640;
    int height = 480;
    std::vector<float> pixelValues;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "My RayTracer", NULL, NULL);
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
    for (int i = 0; i < width * height * 4; i++) {
        pixelValues.push_back(dist(rng));
    }

    

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        auto start = std::chrono::high_resolution_clock::now();
        glDrawPixels(width, height, GL_RGBA, GL_FLOAT, pixelValues.data());
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