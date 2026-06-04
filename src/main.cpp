
#include <GLFW/glfw3.h>
#include "vector"
#include "../header/common.h"

#include <chrono>

#include "../header/camera.h"
#include "../header/hittable.h"
#include "../header/hittable_list.h"
#include "../header/sphere.h"


#define IMAGE_WIDTH_CONST 800


int main(void)
{
    GLFWwindow* window;
    int renderTime = 0;
    std::vector<float> pixelValues;

    // add objects to the scene(world)
    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = IMAGE_WIDTH_CONST;
    cam.samples_per_pixel = 20;
    cam.max_depth = 10;

    // to Calculate the render time
    auto start = std::chrono::high_resolution_clock::now(); // start the clock
    cam.render(world, pixelValues);
    auto end = std::chrono::high_resolution_clock::now(); // end the clock
    renderTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(); // calculate the time taken


    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    int image_width = IMAGE_WIDTH_CONST;
    int image_height = int(IMAGE_WIDTH_CONST / cam.aspect_ratio);

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
    
    // main game looop
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // draw pixels on the screen from the pixel values array
        glDrawPixels(image_width, image_height, GL_RGB, GL_FLOAT, pixelValues.data());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    std::cout << renderTime <<"ms"; // render time will be printed after the window is closed for now. !NEED TO SHOW ON THE SCREEN!

    glfwTerminate();
    return 0;
}