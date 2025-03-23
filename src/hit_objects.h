#ifndef HIT_OBJECTS_H
#define HIT_OBJECTS_H

#include "it_hit.h"
#include "raytracing.h"

#include<vector>

using namespace std;


class hittable_list : public did_it_hit {
public:
	vector<shared_ptr<did_it_hit>> objects;

	hittable_list() {}
	hittable_list(shared_ptr<did_it_hit> object) { add(object);}

	void clear() { objects.clear();}

	void add(shared_ptr<did_it_hit> object) {
			objects.push_back(object);
	}

	bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_ah& rec) const override {
		hit_ah temp_rec;
		bool hit_anything = false;
		auto closet_so_far = ray_tmax;

		for(const auto& object : objects)
		{
			if(object->hit(r, ray_tmin, closet_so_far, temp_rec)) {
				hit_anything = true;
				closet_so_far	 = temp_rec.t;
				rec = temp_rec;
			}
		}

		return hit_anything;
	}


};

#endif