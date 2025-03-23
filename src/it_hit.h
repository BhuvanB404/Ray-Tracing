#ifndef IT_HIT_H
#define IT_HIT_H

#include "raytracing.h"

class hit_ah{
public:
	point3 p;
	vec3 normal;
	double t;
	bool front_face;

	void set_face_normal(const ray& r, const vec3& outward_normal) {
		//hit record normal vector

		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal: -outward_normal ;
		
	}

};

class did_it_hit{
public:
	virtual ~did_it_hit() = default;

	virtual bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_ah& rec) const = 0;
};

#endif

