#include<stdio.h>
#include<SDL2/SDL.h>
#include<math.h>


#define WIDTH 1200
#define HEIGHT 900
#define M_PI 3.14159265358979323846
#define COLOR_WHITE 0xffffffff
#define COLOR_BLACK 0x00000000
#define COLOR_OF_RAY 0xffd43b
#define COLOR_RAY_BLUR 0xbd6800
#define Ray_THICK  1
#define RAYS_NUMBER 100

struct Circle
{
    double x;
    double y;
    double radius;
    /* data */
};

struct Ray
{
    double firstx, firsty;
    double angleOfRay;
    // double lastx, lasty;
};

void FillCircle(SDL_Surface* surface, struct Circle circle, Uint32 color)
{
    double rsquare = pow(circle.radius, 2);
    for(double x = circle.x - circle.radius; x <= circle.x + circle.radius; x++)
    {
        for(double y = circle.y - circle.radius; y <= circle.y + circle.radius ;y++)
        {
            double discentersquare = pow(x - circle.x, 2) + pow( y - circle.y, 2);

            if(discentersquare < rsquare)
            {
                SDL_Rect pixel = (SDL_Rect){x,y,1,1};
                SDL_FillRect(surface, &pixel, color);
            }
        }
        
    }
}


void raysGen(struct Circle circle, struct Ray rays[RAYS_NUMBER] )
{   
    for(int  i =0; i < RAYS_NUMBER; i++)
   { // {
    //     double angle =  ( (double)i / RAYS_NUMBER) * 2 * M_PI;
    //     struct Ray ray = {circle.x, circle.y, angle };
    //     rays[i] = ray;
    //     // printf("%f\n", angle)
        double angle = ((double)i / RAYS_NUMBER) * 2 * M_PI;
        rays[i].firstx = circle.x;
        rays[i].firsty = circle.y;
        rays[i].angleOfRay = angle;
        
    }
}

void FillRays(SDL_Surface* surface, struct Ray rays[RAYS_NUMBER], Uint32 color , struct Circle object, Uint32 blur_color)
{       
        double radius_s = pow(object.radius, 2);
        for(int i = 0; i < RAYS_NUMBER ; i++ )
        {
            struct Ray ray = rays[i];
            printf("Ray %d: Starting at (%f, %f) with angle %f\n", 
                i, ray.firstx, ray.firsty, ray.angleOfRay);

            int end_of_screen = 0;
            int object_hit  = 0;

            double step = 1;
            double x_draw = ray.firstx;
            double y_draw = ray.firsty;
            // while (!end_of_screen && !object_hit)
            // {
            //      x_draw +=  step * cos(ray.angleOfRay);
            //      y_draw +=  step * sin(ray.angleOfRay);

            //     SDL_Rect pixel = (SDL_Rect){x_draw, y_draw, 1, 1};
            //     SDL_FillRect(surface, &pixel, color);

            //     if(x_draw < 0 || x_draw > WIDTH)
            //     {
            //         end_of_screen = 1;
                
            //     }

            //     if(y_draw < 0  || y_draw > HEIGHT)
            //     {
            //         end_of_screen = 1;
            //     }

            // }



            while(x_draw >= 0 && x_draw < WIDTH && y_draw >= 0 && y_draw < HEIGHT)
        {
            SDL_Rect ray_pixel = {(int)x_draw, (int)y_draw, Ray_THICK, Ray_THICK};
            double blur = 1.5 *Ray_THICK;
            SDL_Rect ray_blur = {x_draw,y_draw, blur, blur};
            SDL_FillRect(surface, &ray_blur, blur_color);
            SDL_FillRect(surface, &ray_pixel, color);

            
            x_draw += step * cos(ray.angleOfRay);
            y_draw += step * sin(ray.angleOfRay);

            // does ray hit object
            double discentersquare = pow(x_draw - object.x, 2) + pow( y_draw - object.y, 2);

            if(discentersquare < radius_s)
            {
                break;
            }
      
        }
    
        }
}

void move_circleShadow(struct Circle* circle_shadow)
{
    circle_shadow->y += 1;
    
}


int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window*  window = SDL_CreateWindow("Raytracing", SDL_WINDOWPOS_CENTERED,   
    SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);

    SDL_Surface* surface = SDL_GetWindowSurface(window);

    struct Circle circle = {150,100, 50};
    SDL_Rect erase_rectangle  = {0, 0, WIDTH, HEIGHT };
    struct Circle circleShadow = {410, 400, 120};

    struct Ray ray[RAYS_NUMBER];
    
   int obstacle_speed =4;

    int simrun = 1; 
    SDL_Event event;
    while(simrun)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type ==SDL_QUIT)
            {
                simrun = 0;
            }
            if(event.type == SDL_MOUSEMOTION && event.motion.state != 0)
            {
                circle.x = event.motion.x;
                circle.y = event.motion.y;
                raysGen(circle, ray);

            }
        
        }
        SDL_FillRect(surface, &erase_rectangle, COLOR_BLACK);
        FillRays(surface, ray, COLOR_OF_RAY,  circleShadow, COLOR_RAY_BLUR);

        FillCircle(surface, circle,  COLOR_WHITE);

        FillCircle(surface, circleShadow,  COLOR_WHITE);

            circleShadow.y += obstacle_speed;
        // move_circleShadow(&circleShadow);
        if(circleShadow.y - circleShadow.radius < 0)
        {
            obstacle_speed = -obstacle_speed;
        }
        if(circleShadow.y + circleShadow.radius > HEIGHT )
        {
            obstacle_speed = -obstacle_speed;
        }



        SDL_UpdateWindowSurface(window);
        SDL_Delay(10);
    }



    
}

