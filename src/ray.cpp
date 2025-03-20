#include<iostream>

#include "vec3.h"
#include "color.h"
#include "ray.h"


using namespace std;

color ray_c(const ray& r)
{
	vec3 direction = unit_vec(r.direction());

	auto a = 0.5 * (direction.y() + 1.0);
	return (1.0 -a) * color(1.0,1.0,1.0) + a*color(0.5, 0.7, 1.0);
}

#include<iostream>
#include<fstream>
#include "vec3.h"
#include "color.h"
#include "ray.h"
using namespace std;

// color ray_c(const ray& r)
// {
//     vec3 direction = unit_vec(r.direction());
//     auto a = 0.5 * (direction.y() + 1.0);
//     return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
// }

// void write_color(std::ostream& out, const color& pixel_color) {
//     // Make sure we're outputting valid integer values between 0-255
//     int r = static_cast<int>(255.999 * pixel_color.x());
//     int g = static_cast<int>(255.999 * pixel_color.y());
//     int b = static_cast<int>(255.999 * pixel_color.z());
    
//     // Clamp values to ensure they're in the valid range
//     r = (r < 0) ? 0 : ((r > 255) ? 255 : r);
//     g = (g < 0) ? 0 : ((g > 255) ? 255 : g);
//     b = (b < 0) ? 0 : ((b > 255) ? 255 : b);
    
//     // Output with no trailing spaces except for the required newline
//     out << r << ' ' << g << ' ' << b << '\n';
// }

int main()
{
    // Image
    auto aspect_ratio = 16.0 / 9.0;
    int im_width = 400;
    int im_height = static_cast<int>(im_width / aspect_ratio);
    im_height = (im_height < 1) ? 1 : im_height;
    
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
            color pixel_color = ray_c(r);
            
            // Use our custom write_color directly (don't rely on the external one)
            write_color(outfile, pixel_color);
        }
    }
    
    outfile.close();
    std::cerr << "\rDone.                 \n";
    
    return 0;
}


// int main()
// {
// 	//image
	
// 	auto aspect_ratio = 16.0 / 9.0;
// 	int im_width = 400;

// 	int im_height = int(im_width / aspect_ratio);
// 	im_height = (im_height < 1) ? 1 : im_height;

// 	//camera
// 	//
// 	auto focal_l = 1.0;
// 	auto view_h = 2.0;
// 	auto view_w = view_h * (double(im_width) / im_height);
// 	auto camera_center = point3(0,0,0);

// 	//horitzontal and veritacal vectors
// 	auto view_u = vec3(view_w, 0 , 0);
// 	auto view_v  = vec3(0, -view_h, 0);


// 	//delta vectors between adjacent pixels
	
// 	auto pixel_d_u =  view_u / im_width;
// 	auto pixel_d_v = view_v / im_height;

// 	//top left pixel
	
// 	auto view_topleft =  camera_center - vec3(0, 0 , focal_l) - view_u/2 - view_v/2;


// 	auto pixel_loc = view_topleft + 0.5 * (pixel_d_u +pixel_d_v);

// 	//render pls

// 	 cout << "P3\n" << im_width << " " << im_height << "\n255\n";

// 	for(int j = 0 ; j < im_height; j++)
// 	{	
// 		std::clog << "\rScanlines remaining: " << (im_height - j) << ' '<< std::flush;
// 		for(int i = 0; i < im_width; i++)
// 		{
// 			auto pixel_toCenter = pixel_loc + ( i * pixel_d_u) + ( j* pixel_d_v) ;
// 			auto ray_dir = pixel_toCenter - camera_center;
// 			ray r(camera_center, ray_dir);
// 			// clog << "\r pixel_toCenter :" << pixel_toCenter << "\r ray diretopm: " << ray_dir;  	
// 			color pixel_color = ray_c(r);
// 			write_color(cout, pixel_color);
// 		}

// 	}
// std::clog << "\rDone.                 \n";

// }

