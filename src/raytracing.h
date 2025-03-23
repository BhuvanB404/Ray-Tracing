#ifndef RAYTRACING_H
#define RAYTRACING_H

#include<cmath>
#include <iostream>
#include <limits>
#include <memory>

//C++ std:: using terms

using std::make_shared;
using std::shared_ptr;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

//Utility functions

inline double degree_to_radians(double degrees) {
	return degrees* pi/180.0;
}

//Comman headers

#include "color.h"
#include "ray.h"
#include "vec3.h"

#endif
