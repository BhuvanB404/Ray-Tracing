#ifndef OBJECTS_H
#define OBJECTS_H

#include "it_hit.h"
#include "material.h"
#include "ray.h"
#include "vec3.h"
#include "rtweekend.h"

class sphere : public did_it_hit {
  public:
    sphere(const point3& center, double radius)
        : center(center), radius(std::fmax(0, radius)) {}

    sphere(const point3& center, double radius, std::shared_ptr<material> mat)
        : center(center), radius(std::fmax(0, radius)), mat(mat) {}

    bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const override {
        vec3 oc = r.origin() - center;
        double a = r.direction().length_squared();
        double half_b = dot(oc, r.direction());
        double c = oc.length_squared() - radius*radius;

        double discriminant = half_b*half_b - a*c;
        if (discriminant < 0) return false;
        double sqrtd = sqrt(discriminant);

        // root 
        double root = (-half_b - sqrtd) / a;
        if (root <= ray_tmin || ray_tmax <= root) {
            root = (-half_b + sqrtd) / a;
            if (root <= ray_tmin || ray_tmax <= root)
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        vec3 outward_normal = (rec.p - center) / radius;
        

        double theta = acos(-outward_normal.y());
        double phi = atan2(-outward_normal.z(), outward_normal.x()) + pi;
        rec.u = phi / (2*pi);
        rec.v = theta / pi;
        
        rec.set_face_normal(r, outward_normal);
        rec.mat = mat;

        return true;
    }

  private:
    point3 center;
    double radius;
    std::shared_ptr<material> mat;
};

#endif