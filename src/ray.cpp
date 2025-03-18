#include<iostream>

#include "vec3.h"
#include "color.h"


using namespace std;

int main()
{
	int im_width = 256;
	int im_height = 256;


	//image render
	cout << "P3 \n" << im_width << ' ' << im_height << "\n255\n";


	for(int j = 0 ; j < im_height; j++)
	{	std::clog << "\rScanlines remaining: " << (im_height - j) << ' '<< std::flush;
		for(int i = 0; i < im_width; i++)
		{
			//auto r = double(i) / (im_width - 1);9
			//auto g =  double(j) / (im_height - 1);
			//auto b  = 0.0;
			//int ir = int(255.999 * r);
			//int ig = int(255.999 * g);
			//int ib = int(255.999 * b);
			//cout << ir << ' ' << ig << ' ' << ib << endl;
			auto pixel_color = color(double(i)/(image_width - 1), double(j)/(image_height -1), 0);
			write_color(std::cout, pixel_color);
		}
	}

	std::clog << "\rDome.			\n";
}
