#ifndef HIT_OBJECTS_H
#define HIT_OBJECTS_H

#include "it_hit.h"
#include "interval.h"
#include "raytracing.h"

#include <vector>

class hittable_list : public did_it_hit {
public:
    std::vector<std::shared_ptr<did_it_hit>> objects;

    hittable_list() {}
    hittable_list(std::shared_ptr<did_it_hit> object) { add(object); }

    void clear() { objects.clear(); }

    void add(std::shared_ptr<did_it_hit> object) {
        objects.push_back(object);
    }

    bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const override {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_tmax;

        for (const auto& object : objects) {
            if (object->hit(r, ray_tmin, closest_so_far, temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }

        return hit_anything;
    }
};

#endif