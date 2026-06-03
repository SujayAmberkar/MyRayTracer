
#include <GLFW/glfw3.h>
#include "vector"
#include "common.h"
#include <random>
#include <chrono>

#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

#define IMAGE_WIDTH_CONST 800


color ray_color(const ray& r, const hittable& world) {
    
    hit_record rec;
    if (world.hit(r, 0, infinity, rec)) {
        return 0.5 * (rec.normal + color(1, 1, 1));
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}

void GenerateImage(int image_width, int image_height,
                    vec3 pixel00_loc, vec3 camera_center,
                    vec3 pixel_delta_u, vec3 pixel_delta_v,
                    std::vector<float> &pixelValues, const hittable& world) {
    // create a color array
    for (int j = image_height - 1; j >= 0; j--) {
        for (int i = 0; i < image_width; i++) {
            auto pixel_center = pixel00_loc + i * pixel_delta_u + j * pixel_delta_v;
            auto ray_direction = pixel_center - camera_center;

            ray r(camera_center, ray_direction);
            auto pixel_color = ray_color(r, world);

            pixelValues.push_back((float)pixel_color.x()); // R
            pixelValues.push_back((float)pixel_color.y()); // G
            pixelValues.push_back((float)pixel_color.z()); // B

        }
    }
}

int main(void)
{
    GLFWwindow* window;
    int renderTime = 0;

    auto aspect_ratio = 16.0 / 9.0;
    int image_width = IMAGE_WIDTH_CONST;
    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height; // ensure image height is atleast 1

    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width) / image_height);

    // horizontal and vertical vectors for the viewport
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    // calculate the delta vectors for each pixel. these are equal to the length of 1 pixel
    // These help in moving to the next pixel. 
    auto pixel_delta_u = viewport_u / (image_width);
    auto pixel_delta_v = viewport_v / (image_height);

    // camera 
    auto camera_center = point3(0, 0, 0);
    auto focal_length = 1.0;

    // position of the upper left corner of the viewport
    auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u / 2.0 - viewport_v / 2.0;
    auto pixel00_location = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // world
    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));



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

    GenerateImage(image_width, image_height , pixel00_location, camera_center, pixel_delta_u, pixel_delta_v, pixelValues, world);
    
    // main game looop
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // to Calculate the render time
        
        auto start = std::chrono::high_resolution_clock::now(); // start the clock

        // draw pixels on the screen from the pixel values array
        glDrawPixels(image_width, image_height, GL_RGB, GL_FLOAT, pixelValues.data());
        auto end = std::chrono::high_resolution_clock::now(); // end the clock
        renderTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(); // calculate the time taken


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    std::cout << renderTime <<"ms"; // render time will be printed after the window is closed for now. !NEED TO SHOW ON THE SCREEN!

    glfwTerminate();
    return 0;
}