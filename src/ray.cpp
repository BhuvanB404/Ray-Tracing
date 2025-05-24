#include "raytracing.h"
#include "hit_objects.h"
#include "objects.h"
#include "material.h"
#include "color.h"

#include <fstream>
#include <memory>
#include <vector>
#include <string>
#include <sstream>

color ray_c(const ray& r, const did_it_hit& world, int depth = 50) {
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered
    if (depth <= 0)
        return color(0, 0, 0);

    if (!world.hit(r, 0.001, infinity, rec))
        return color(0.5, 0.7, 1.0);

    ray scattered;
    color attenuation;
    color emitted = rec.mat->emitted(rec.u, rec.v, rec.p);

    if (!rec.mat->scatter(r, rec, attenuation, scattered))
        return emitted;

    return emitted + attenuation * ray_c(scattered, world, depth-1);
}

int main() {
    // World
    hittable_list world;

    // Ground
    auto ground_material = std::make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(std::make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    // Random small spheres
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // Diffuse
                    auto albedo = color(random_double() * random_double(),
                                      random_double() * random_double(),
                                      random_double() * random_double());
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // Metal
                    auto albedo = color(0.5 * (1 + random_double()),
                                      0.5 * (1 + random_double()),
                                      0.5 * (1 + random_double()));
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // Glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    // Three main spheres
    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = std::max(1, static_cast<int>(image_width / aspect_ratio));

    // Open output file
    std::ofstream outfile("image.ppm");
    if (!outfile) {
        std::cerr << "Error: Failed to open output file\n";
        return 1;
    }
    
    // Camera
    point3 lookfrom(13, 2, 3);
    point3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;
    auto vfov = 20.0;

    // Camera setup
    const auto theta = degrees_to_radians(vfov);
    const auto h = tan(theta/2);
    const auto viewport_height = 2.0 * h;
    const auto viewport_width = aspect_ratio * viewport_height;
    
    const auto w = unit_vector(lookfrom - lookat);
    const auto u = unit_vector(cross(vup, w));
    const auto v = cross(w, u);
    
    const auto origin = lookfrom;
    const auto horizontal = dist_to_focus * viewport_width * u;
    const auto vertical = dist_to_focus * viewport_height * v;
    const auto lower_left_corner = origin - horizontal/2 - vertical/2 - dist_to_focus*w;
    
    // Render settings
    const int samples_per_pixel = 100;
    const int max_depth = 50;
    
    // Render to file
    outfile << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width-1);
                auto v = (j + random_double()) / (image_height-1);
                
                // Create ray from camera through pixel
                ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);
                pixel_color += ray_c(r, world, max_depth);
            }
            write_color(outfile, pixel_color, samples_per_pixel);
        }
    }
    
    outfile.close();
    std::cerr << "\rDone.                 \n";
    
    return 0;
}