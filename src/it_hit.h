#ifndef IT_HIT_H
#define IT_HIT_H

#include "raytracing.h"

// Forward declaration
class material;

class hit_record {
public:
    point3 p;                     // Point where the ray hit the object
    vec3 normal;                  // Surface normal at the hit point
    std::shared_ptr<material> mat; // Material of the hit object
    double t;                     // Ray parameter at the hit point
    double u, v;                  // Texture coordinates
    bool front_face;              // Whether the ray hit the front face

    // Set the face normal based on the ray direction and outward normal
    void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

// Abstract base class for hittable objects
class did_it_hit {
public:
    virtual ~did_it_hit() = default;

    // Check if a ray hits the object between ray_tmin and ray_tmax
    // If hit, populate the hit_record and return true
    virtual bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const = 0;
};

#endif

