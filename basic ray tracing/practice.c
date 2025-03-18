#include<stdio.h>
#include<SDL2/SDL.h>
#include<math.h>

#define WIDTH 1200
#define HEIGHT 900
#define M_PI 3.14159265358979323846
#define COLOR_WHITE 0xffffffff
#define COLOR_WHITE 0xffffffff
#define COLOR_BLACK 0x00000000
#define COLOR_OF_RAY 0xffd43b
#define COLOR_RAY_BLUR 0xbd6800
#define RAY_THICKNESS  2
#define RAYS_NUMBER 100

typedef struct 
{
	double x;
	double y;
	double radius;
}Circle;

typedef struct {
	double firstx,firsty;
	double RayAngle;
	//double lastx,lasty;
}Ray;

void FillCircle(SDL_Surface* surface, Circle circle, Uint32 color)
{
	double rsquare = pow(circle.radius, 2);
	for(double x = circle.x -  circle.radius; x <= circle.x +circle.radius; x++)
	{
		for(double y = circle.y -  circle.radius; y <= circle.y + circle.radius; y++)
		{
			double DcentreSquare = pow(x-circle.x,2) + pow(y - circle.y, 2);
			{
				if(DcentreSquare < rsquare)
				{
					SDL_Rect pixel = (SDL_Rect){x,y,1,1};
					SDL_FillRect(surface, &pixel, color);
				}
			}
		}
	}

}

void rayGen(Circle circle, Ray ray[RAYS_NUMBER] )
{
	for(int i = 0; i < RAYS_NUMBER; i++)
	{
		double angle = ((double)i / RAYS_NUMBER) * 2 * M_PI;
		ray[i].firstx = circle.x;
		ray[i].firsty = circle.y;
		ray[i].RayAngle = angle;
	}
}

void FillRay(SDL_Surface* surface, Ray rays[RAYS_NUMBER] ,Uint32 color, Circle object)
{
	double radiusSquared = pow(object.radius,2);
	for(int i = 0; i < RAYS_NUMBER; i++ )
	{
		Ray ray =  rays[i];
		printf("Ray %d: position (%f %f ) with angle %f\n", i, ray.firstx, ray.firsty,ray.RayAngle);

		int eos = 0;
		int object_hit = 0;

		double step = 1;
		double x_draw = ray.firstx;
		double y_draw = ray.firsty;
		
		while(x_draw >= 0 && x_draw < WIDTH && y_draw >= 0  && y_draw < HEIGHT )
		{

			SDL_Rect ray_pixel = {(int) x_draw, (int) y_draw, RAY_THICKNESS, RAY_THICKNESS};
			SDL_FillRect(surface, &ray_pixel, color);

			x_draw += step * cos(ray.RayAngle);
			y_draw += step * sin(ray.RayAngle);

			double DcentreSquare = pow(x_draw - object.x, 2) + pow(y_draw - object.y, 2);

			if(DcentreSquare <  radiusSquared)
			{
				break;
			}
		}
	}


}

int main()
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("RayTracing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);

	SDL_Surface* surface = SDL_GetWindowSurface(window);

	Circle circle = { 150,100, 50};
	SDL_Rect erase_rectangle = { 0, 0 , WIDTH, HEIGHT};
	Circle circleShadow = { 410, 400, 120 };

	Ray ray[RAYS_NUMBER];

	int obstacle_speed = 5;
	int simrun = 2;
	SDL_Event event;
	rayGen(circle, ray);
	
	while(simrun)
{
    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
        {
            simrun = 0;
        }
        else if(event.type == SDL_MOUSEMOTION && event.motion.state != 0)
        {
            circle.x = event.motion.x;
            circle.y = event.motion.y;
            rayGen(circle, ray);
        }
    }

	SDL_FillRect(surface, &erase_rectangle, COLOR_BLACK);
	FillRay(surface, ray, COLOR_OF_RAY, circleShadow);

	FillCircle(surface, circle, COLOR_WHITE);

	FillCircle(surface, circleShadow, COLOR_WHITE);

	circleShadow.y += obstacle_speed;

	if(circleShadow.y - circleShadow.radius < 0)
	{
		obstacle_speed = -obstacle_speed;
	}
	if(circleShadow.y +  circleShadow.radius > HEIGHT)
	{
		obstacle_speed = -obstacle_speed;
	}


	SDL_UpdateWindowSurface(window);
	SDL_Delay(10);
	}
}
	

	

	
	
	
	
