#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"
#include <vector>

class camera {
public:
	double aspect_ratio = 16.0 / 9.0;
	int image_width = 100;
	int samples_per_pixel = 10;	// count random samples for each pixel
	int max_depth = 10;
	

	void render(const hittable& world, std::vector<float>& pixelValues) {
		initialize();

		// create a color array
		for (int j = image_height - 1; j >= 0; j--) {
			for (int i = 0; i < image_width; i++) {
				color pixel_color(0, 0, 0);
				for (int sample = 0; sample < samples_per_pixel; sample++) {
					ray r = get_ray(i, j);
					pixel_color += ray_color(r, max_depth, world);
				}
				pixelValues.push_back((float)pixel_color.x() * pixel_samples_scale); // R
				pixelValues.push_back((float)pixel_color.y() * pixel_samples_scale); // G
				pixelValues.push_back((float)pixel_color.z() * pixel_samples_scale); // B
			}
		}
	}

private:
	int image_height;
	point3 camera_center;
	point3 pixel00_location;
	vec3 pixel_delta_u;
	vec3 pixel_delta_v;
	double pixel_samples_scale;

	void initialize() {
		image_height = int(image_width / aspect_ratio);
		image_height = (image_height < 1) ? 1 : image_height; // ensure image height is atleast 1
		pixel_samples_scale = 1.0 / samples_per_pixel;

		// camera 
		camera_center = point3(0, 0, 0);

		// Determine viewport dimensions.
		auto focal_length = 1.0;
		auto viewport_height = 2.0;
		auto viewport_width = viewport_height * (double(image_width) / image_height);

		// horizontal and vertical vectors for the viewport
		auto viewport_u = vec3(viewport_width, 0, 0);
		auto viewport_v = vec3(0, -viewport_height, 0);

		// calculate the delta vectors for each pixel. these are equal to the length of 1 pixel
		// These help in moving to the next pixel. 
		pixel_delta_u = viewport_u / (image_width);
		pixel_delta_v = viewport_v / (image_height);

		// position of the upper left corner of the viewport
		auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u / 2.0 - viewport_v / 2.0;
		pixel00_location = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
	}

	ray get_ray(int i, int j) const{
		auto offset = sample_square();
		auto pixel_sample = pixel00_location
			+ ((i + offset.x()) * pixel_delta_u)
			+ ((j + offset.y()) * pixel_delta_v);

		auto ray_origin = camera_center;
		auto ray_direction = pixel_sample - ray_origin;
		return ray(ray_origin, ray_direction);
	}

	vec3 sample_square() const{
		return vec3(random_double() - 0.5, random_double() - 0.5, 0);
	}

	color ray_color(const ray& r,int depth, const hittable& world) {
		// if the ray has bounced enough then stop
		if (depth <= 0)
			return color(0, 0, 0);

		// record intersection data
		hit_record rec;
		if (world.hit(r, interval(0.001, infinity), rec)) {
			ray scattered;
			color attenuation;

			if (rec.mat->scatter(r, rec, attenuation, scattered))
				return attenuation * ray_color(scattered, depth - 1, world);
			return color(0, 0, 0);
		}

		vec3 unit_direction = unit_vector(r.direction());
		auto a = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
	}
};

#endif