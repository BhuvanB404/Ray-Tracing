#include "raytracing.h"

#include "it_hit.h"
#include "objects.h"
#include "hit_objects.h"

#include<fstream>

using namespace std;

color ray_c(const ray& r, const did_it_hit& world)
{
    hit_ah rec;
    if(world.hit(r, 0, infinity, rec)){
        return 0.5 * (rec.normal + color(1 , 1, 1));

    }


    vec3 direction = unit_vec(r.direction());

    auto a = 0.5 * (direction.y() + 1.0);
    return (1.0 -a) * color(1.0,1.0,1.0) + a*color(0.5, 0.7, 1.0);
}



// color ray_c(const ray& r)
// {
//     vec3 direction = unit_vec(r.direction());
//     auto a = 0.5 * (direction.y() + 1.0);
//     return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
// }



int main()
{
    // Image
    auto aspect_ratio = 16.0 / 9.0;
    int im_width = 400;
    int im_height = static_cast<int>(im_width / aspect_ratio);
    im_height = (im_height < 1) ? 1 : im_height;

    // World

    hittable_list world;

    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

    
    // Open file for writing (instead of cout)
    std::ofstream outfile("image.ppm");
    if (!outfile.is_open()) {
        std::cerr << "Failed to open output file" << std::endl;
        return 1;
    }
    
    // Camera
    auto focal_l = 1.0;
    auto view_h = 2.0;
    auto view_w = view_h * (double(im_width) / im_height);
    auto camera_center = point3(0, 0, 0);
    
    // Horizontal and vertical vectors
    auto view_u = vec3(view_w, 0, 0);
    auto view_v = vec3(0, -view_h, 0);
    
    // Delta vectors between adjacent pixels
    auto pixel_d_u = view_u / im_width;
    auto pixel_d_v = view_v / im_height;
    
    // Top left pixel
    auto view_topleft = camera_center - vec3(0, 0, focal_l) - view_u/2 - view_v/2;
    auto pixel_loc = view_topleft + 0.5 * (pixel_d_u + pixel_d_v);
    
    // Write PPM header - use spaces consistently
    outfile << "P3\n" << im_width << " " << im_height << "\n255\n";
    
    for(int j = 0; j < im_height; j++)
    {    
        std::cerr << "\rScanlines remaining: " << (im_height - j) << ' ' << std::flush;
        for(int i = 0; i < im_width; i++)
        {
            auto pixel_center = pixel_loc + (i * pixel_d_u) + (j * pixel_d_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);
            // color pixel_color = ray_c(r);
            

            color pixel_color = ray_c(r, world);            
            write_color(outfile, pixel_color);
        }
    }
    
    outfile.close();
    std::cerr << "\rDone.                 \n";
    
    return 0;
}


