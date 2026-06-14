#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <GLFW/glfw3.h>
#include "vector"
#include "../header/common.h"

#include <chrono>

#include "../header/camera.h"
#include "../header/hittable.h"
#include "../header/hittable_list.h"
#include "../header/material.h"
#include "../header/sphere.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


#define IMAGE_WIDTH_CONST 800

void SaveRender(std::vector<float> pixelValues, int image_width, int image_height) {
    std::vector<unsigned char> pngPixels(pixelValues.size());
    for (int i = 0; i < pixelValues.size(); i++) {
        pngPixels[i] = (unsigned char)(pixelValues[i] * 255.99f);
    }
    stbi_flip_vertically_on_write(1);
    stbi_write_png("render.png", image_width, image_height, 3, pngPixels.data(), image_width * 3);
}

hittable_list SampleScene(hittable_list world) {
    auto material_ground = make_shared<lambertian>(color(0.74, 0.76, 0.78));


    auto material_center = make_shared<lambertian>(color(0.9, 0.29, 0.235));
    
    // glass
    auto outer_glass_mat = make_shared<dielectric>(1.50);
    auto inner_glass_mat = make_shared<dielectric>(1.00 / 1.50);
    
    // smooth metal
    auto smooth_metal_mat = make_shared<metal>(color(0.8, 0.6, 0.2), 0.01);

    // fuzzy metal
    auto fuzzy_metal_mat = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3(0.0, 0.2, -2.2), 0.7, material_center));

    //hollow glass
    world.add(make_shared<sphere>(point3(-0.8, 0.0, -1.0), 0.5, outer_glass_mat));
    world.add(make_shared<sphere>(point3(-0.8, 0.0, -1.0), 0.4, inner_glass_mat));

    world.add(make_shared<sphere>(point3(1.5, 0.2, -1.0), 0.7, smooth_metal_mat));
    world.add(make_shared<sphere>(point3(0.0, -0.2, 0.0), 0.3, fuzzy_metal_mat));
    return world;
}

hittable_list SampleScene2(hittable_list world) {
    auto R = std::cos(pi / 4);

    auto material_left = make_shared<lambertian>(color(0, 0, 1));
    auto material_right = make_shared<lambertian>(color(1, 0, 0));

    world.add(make_shared<sphere>(point3(-R, 0, -1), R, material_left));
    world.add(make_shared<sphere>(point3(R, 0, -1), R, material_right));
    return world;
}

int main(void)
{
    GLFWwindow* window;
    int renderTime = 0;
    std::vector<float> pixelValues;

    // add objects to the scene(world)
    hittable_list world;
    world = SampleScene(world);


    // Camera setup
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = IMAGE_WIDTH_CONST;
    cam.samples_per_pixel = 50;
    cam.max_depth = 20;
    cam.vfov = 30;
    cam.lookfrom = point3(-1, 1, 3);
    cam.lookat = point3(0, 0, -1);
    cam.vup = vec3(0, 1, 0);

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

    // Setup ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    
    // main game looop
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        

        // draw pixels on the screen from the pixel values array
        glDrawPixels(image_width, image_height, GL_RGB, GL_FLOAT, pixelValues.data());

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        ImGui::Begin("Stats");
        ImGui::Text("Render Time: %dms", renderTime);
        ImGui::SliderInt("Samples", &cam.samples_per_pixel, 1, 100);
        ImGui::SliderInt("Max Depth", &cam.max_depth, 1, 50);
        if (ImGui::Button("Render")) {
            pixelValues.clear();
            cam.render(world, pixelValues);
        }
        if (ImGui::Button("Save Render")) {
            SaveRender(pixelValues, image_width, image_height);
        }
        ImGui::End();


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    std::cout << renderTime <<"ms"; // render time will be printed after the window is closed for now. !NEED TO SHOW ON THE SCREEN!
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}